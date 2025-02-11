#include "../include/mainwindow.h"
#include "../include/VideoCombiner.h"
#include "../include/VideoTable.h"
#include "QInputDialog"
#include "QFormLayout"
#include "QLineEdit"
#include "QSpinBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoTable(new VideoTable(this))
    , timeLine(new TimeLine(this))
    , options(new Options)
    , isProjectSaved(true)
{
    ui->setupUi(this);
    this->setWindowTitle("Video Editor");
    this->videoPreviewWidget = new VideoPreview(this->timeLine, this);


    QVBoxLayout *layout = new QVBoxLayout(ui->groupBoxVideo);
    layout->addWidget(videoPreviewWidget);
    ui->groupBoxVideo->setLayout(layout);


    QVBoxLayout *videoTableLayout = new QVBoxLayout(ui->groupBoxVideosTable);
    videoTableLayout->addWidget(videoTable);
    ui->groupBoxVideosTable->setLayout(videoTableLayout);


    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(timeLine);
    scrollArea->setWidgetResizable(true);
    ui->timeLineScrollArea->setLayout(new QVBoxLayout);
    ui->timeLineScrollArea->layout()->addWidget(scrollArea);


    positionUpdateTimer = new QTimer(this);
    positionUpdateTimer->start(1000);

    ui->volumeChangeSlider->setRange(0, 100);
    ui->volumeChangeSlider->setValue(50);

    connect(ui->volumeChangeSlider, &QSlider::valueChanged, this, [this](int value) {
        videoPreviewWidget->setVolume(value / 100.0);
    });

    connect(positionUpdateTimer, &QTimer::timeout, this, [this]() {
        updateDurationLabel();
    });


    connect(videoTable, &VideoTable::videoRemoved, timeLine, &TimeLine::removeVideoObjects);
    connect(timeLine, &TimeLine::resetVideoPlayer, videoPreviewWidget, &VideoPreview::resetVideoPlayer);
    connect(videoPreviewWidget, &VideoPreview::playbackFinished, this, [this]() {
    ui->playPauseButton->setText("Play");
    });

    connect(timeLine, &TimeLine::timelineModified, this, [this]() {
        isProjectSaved = false;
    });


    setupShortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playPauseButton_clicked()
{
    if (timeLine->getVideoList().isEmpty()) {
        QMessageBox::warning(this, "Warning", "No videos on the timeline.");
        return;
    }

    if (videoPreviewWidget->getPlaybackState() == QMediaPlayer::PlayingState) {
        videoPreviewWidget->pause();
        ui->playPauseButton->setText("Play");
    } else {
        videoPreviewWidget->play();
        ui->playPauseButton->setText("Pause");
    }
}

void MainWindow::on_volumeChangeSlider_valueChanged(int value)
{
    float volume = static_cast<float>(value) / 100;
    videoPreviewWidget->setVolume(volume);
    ui->volumeLabel->setText(QString::number(value));

    if(volume > 0 && ui->muteButton->text() == "Unmute") {
        ui->muteButton->setText("Mute");
    }
}

void MainWindow::on_muteButton_clicked()
{
    if(videoPreviewWidget->getVolume() > 0) {
        previousVolume = videoPreviewWidget->getVolume();
        videoPreviewWidget->setVolume(0.0);

        ui->muteButton->setText("Unmute");
        ui->volumeChangeSlider->setValue(0);
    }
    else {
        videoPreviewWidget->setVolume(previousVolume);
        ui->muteButton->setText("Mute");
        ui->volumeChangeSlider->setValue(previousVolume * 100);
    }
}

void MainWindow::updateDurationLabel()
{
    if (!timeLine->getVideoList().isEmpty())
    {
        QTime totalTime = QTime::fromMSecsSinceStartOfDay(timeLine->getTotalVideosDurationTime());
        ui->timeLabel->setText(totalTime.toString("hh:mm:ss"));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select video files"), "",
        tr("Video and Audio Files (*.mp4 *.avi *.mov *.mp3 *.wav *.flac)"));
    if (!fileNames.isEmpty()) {
        QList<QString> videoFiles = fileNames.toVector().toList();
        videoTable->updateTable(videoFiles);
        isProjectSaved = false;
    }
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save video file"), "",
                                                    tr("Video Files (*.mp4)"));
    if (!fileName.isEmpty()) {
        QList<VideoData> updatedVideoList;
        for (VideoData &video : timeLine->getVideoList()) {
            video.executeOperations();
            qDebug() << "Exporting video: " << video.getFilePath();
            updatedVideoList.append(video);
        }
        VideoCombiner *combiner = new VideoCombiner(updatedVideoList, fileName, this->options->getResolution(), this->options->getFrameRate() ,this);
        connect(combiner, &VideoCombiner::combineFinished, this, [this](bool success) {
            if (success) {
                QMessageBox::information(this, "Success", "Video exported successfully");
            } else {
                QMessageBox::critical(this, "Error", "Failed to export video");
            }
        });
        combiner->start();
    }
}

void MainWindow::on_actionExport_options_triggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Export Options"));

    QFormLayout form(&dialog);

    QLineEdit *resolutionEdit = new QLineEdit(&dialog);
    resolutionEdit->setText(options->getResolution());
    form.addRow(new QLabel(tr("Resolution (e.g., 1280:720):")), resolutionEdit);

    QSpinBox *frameRateSpinBox = new QSpinBox(&dialog);
    frameRateSpinBox->setRange(1, 120);
    frameRateSpinBox->setValue(options->getFrameRate());
    form.addRow(new QLabel(tr("Frame Rate (fps):")), frameRateSpinBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString resolution = resolutionEdit->text();
        int frameRate = frameRateSpinBox->value();
        options->setResolution(resolution);
        options->setFrameRate(frameRate);
        qDebug() << "Resolution:" << resolution << "Frame Rate:" << frameRate;
    }
}


void MainWindow::on_actionSave_Project_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("JSON Files (*.json)"));
    if (!filePath.isEmpty()) {
        QList<VideoData> timelineVideos = timeLine->getVideoList();
        QList<QString> videoLibrary = videoTable->getLoadedVideos();

        ProjectSaver saver(timelineVideos, videoLibrary, filePath);
        if (saver.saveProject()) {
            QMessageBox::information(this, tr("Success"), tr("Project saved successfully."));
            isProjectSaved = true;
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save project."));
        }
    }
}


void MainWindow::on_actionOpen_Project_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("JSON Files (*.json)"));
    if (!filePath.isEmpty())
    {

        ProjectLoader loader(filePath);
        QList<VideoData> timelineVideos;
        QList<QString> videoLibrary;
        if (loader.loadProject(timelineVideos, videoLibrary))
        {
            for(const auto & timelineVideo : timelineVideos)
            {
                if(!QFile::exists(timelineVideo.getFilePath()))
                {
                    QMessageBox::critical(this, tr("Error"), tr("Failed to load project."));
                    return;
                }
            }

            timeLine->setVideoList(timelineVideos);
            videoTable->updateTable(videoLibrary);
            QMessageBox::information(this, tr("Success"), tr("Project loaded successfully."));
        }

    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isProjectSaved) {
        event->accept();
        return;
    }

    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Program",
        tr("Do you want to save changes before closing?\n"),
        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        on_actionSave_Project_triggered();
        if (isProjectSaved) {
            event->accept();
        } else {
            event->ignore();
        }
    } else if (resBtn == QMessageBox::No) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setupShortcuts()
{
    QShortcut *muteShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_M), this);
    connect(muteShortcut, &QShortcut::activated, this, &MainWindow::on_muteButton_clicked);

    QShortcut *volumeUpShortcut = new QShortcut(QKeySequence(Qt::Key_Up), ui->volumeChangeSlider);
    connect(volumeUpShortcut, &QShortcut::activated, this, [this]() {
        int currentVolume = ui->volumeChangeSlider->value();
        if (currentVolume < 100) {
            ui->volumeChangeSlider->setValue(currentVolume + 5);
        }
    });

    QShortcut *volumeDownShortcut = new QShortcut(QKeySequence(Qt::Key_Down), ui->volumeChangeSlider);
    connect(volumeDownShortcut, &QShortcut::activated, this, [this]() {
        int currentVolume = ui->volumeChangeSlider->value();
        if (currentVolume > 0) {
            ui->volumeChangeSlider->setValue(currentVolume - 5);
        }
    });

    QShortcut *seekForwardShortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(seekForwardShortcut, &QShortcut::activated, this, [this]() {
        qint64 currentPosition = videoPreviewWidget->getPosition();
        videoPreviewWidget->setPosition(currentPosition + 5000);
    });

    QShortcut *seekBackwardShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(seekBackwardShortcut, &QShortcut::activated, this, [this]() {
        qint64 currentPosition = videoPreviewWidget->getPosition();
        videoPreviewWidget->setPosition(currentPosition - 5000);
    });

    QShortcut *pauseShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(pauseShortcut, &QShortcut::activated, this, &MainWindow::on_playPauseButton_clicked);

}