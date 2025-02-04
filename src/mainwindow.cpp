#include "../include/mainwindow.h"
#include "../include/VideoCombiner.h"
#include "../include/VideoTable.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoTable(new VideoTable(this))
    , timeLine(new TimeLine(this))
    , editor(new Editor)
{
    ui->setupUi(this);

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

    ui->previewVideoTimeSlider->setRange(0, 100);
    ui->previewVideoTimeSlider->setValue(0);
    positionUpdateTimer = new QTimer(this);


    connect(positionUpdateTimer, &QTimer::timeout, this, &MainWindow::updateVideoTimeSlider);

    connect(ui->previewVideoTimeSlider, &QSlider::sliderPressed, this, [this]() {
        isSliderBeingMoved = true;
    });

    connect(ui->previewVideoTimeSlider, &QSlider::valueChanged, this, [this](qint64 position) {
        if(isSliderBeingMoved) videoPreviewWidget->setPosition(position * videoPreviewWidget->getDuration() / 100);
    });

    connect(ui->previewVideoTimeSlider, &QSlider::sliderReleased, this, [this]() {
        isSliderBeingMoved = false;

        qint64 duration = videoPreviewWidget->getDuration();
        videoPreviewWidget->setPosition(
            ui->previewVideoTimeSlider->value() * duration / 100
            );
    });

    ui->volumeChangeSlider->setRange(0, 100);
    ui->volumeChangeSlider->setValue(50);
    connect(ui->volumeChangeSlider, &QSlider::valueChanged, this, [this](int value) {
        videoPreviewWidget->setVolume(value / 100.0);
    });

    connect(positionUpdateTimer, &QTimer::timeout, this, [this]() {
        updateDurationLabel();
    });

    connect(videoTable, &VideoTable::playVideoRequested, videoPreviewWidget, &VideoPreview::playVideo);
    connect(videoTable, &VideoTable::videoRemoved, timeLine, &TimeLine::removeVideoObjects);

    connect(videoPreviewWidget, &VideoPreview::playPauseButtonTextChanged, this, [this](const QString &text) {
        ui->playPauseButton->setText(text);
    });

    setupShortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playPauseButton_clicked()
{
    if (videoPreviewWidget->getPlaybackState() == QMediaPlayer::PausedState) {
        videoPreviewWidget->play();
        ui->playPauseButton->setText("Pause");
    } else {
        videoPreviewWidget->pause();
        ui->playPauseButton->setText("Play");
    }
}

void MainWindow::on_volumeChangeSlider_valueChanged(int value)
{
    float volume = static_cast<float>(value) / 100;
    videoPreviewWidget->setVolume(volume);
    ui->volumeLCD->display(value);

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
    if (videoPreviewWidget->getDuration() > 0) {
        qint64 currentPosition = videoPreviewWidget->getPosition();
        qint64 duration = videoPreviewWidget->getDuration();

        QTime currentTime = QTime::fromMSecsSinceStartOfDay(currentPosition);
        QTime totalTime = QTime::fromMSecsSinceStartOfDay(duration);
        ui->timeLabel->setText(currentTime.toString("hh:mm:ss") + " / " + totalTime.toString("hh:mm:ss"));
    }
}



void MainWindow::updateVideoTimeSlider() {
    if (videoPreviewWidget->getDuration() > 0) {
        qint64 currentPosition = videoPreviewWidget->getPosition();
        qint64 duration = videoPreviewWidget->getDuration();
        int sliderPosition = (currentPosition * 100) / duration;

        ui->previewVideoTimeSlider->setValue(sliderPosition);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select video files"), "",
        tr("Video and Audio Files (*.mp4 *.avi *.mov *.mp3 *.wav *.flac)"));
    if (!fileNames.isEmpty()) {
        QList<QString> videoFiles = fileNames.toVector().toList();
        positionUpdateTimer->start(1000);

        videoTable->updateTable(videoFiles);
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

        VideoCombiner *combiner = new VideoCombiner(updatedVideoList, fileName, this);
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


