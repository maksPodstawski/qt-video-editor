#include "../include/mainwindow.h"
#include "../include/VideoCombiner.h"
#include "../include/VideoTable.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , videoPreviewWidget(new VideoPreview(this))
    , videoTable(new VideoTable(this))
    , timeLine(new TimeLine(this))
    , editor(new Editor)
{
    ui->setupUi(this);

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


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playButton_clicked()
{
    videoPreviewWidget->play();
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
}


void MainWindow::on_pauseButton_clicked()
{
    videoPreviewWidget->pause();
    ui->pauseButton->setEnabled(false);
    ui->playButton->setEnabled(true);
}


void MainWindow::on_actionOpen_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select video files"), "",
        tr("Video and Audio Files (*.mp4 *.avi *.mov *.mp3 *.wav *.flac)"));
    if (!fileNames.isEmpty()) {
        QList<QString> videoFiles = fileNames.toVector().toList();
        videoPreviewWidget->setVideoFiles(videoFiles);
        ui->pauseButton->setEnabled(false);
        positionUpdateTimer->start(1000);

        videoTable->updateTable(videoFiles);
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

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save video file"), "",
                                                    tr("Video Files (*.mp4)"));
    if (!fileName.isEmpty()) {
        QList<QString> inputFiles;
        for (const VideoData &video : timeLine->getVideoList()) {
            qDebug() << video.getFilePath();
            inputFiles.append(video.getFilePath());
        }

        VideoCombiner *combiner = new VideoCombiner(editor, inputFiles, fileName, this);
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


