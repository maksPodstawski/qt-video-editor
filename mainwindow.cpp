#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QSlider>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoPreviewWidget = new VideoPreviewWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(ui->groupBoxVideo);
    layout->addWidget(videoPreviewWidget);
    ui->groupBoxVideo->setLayout(layout);

    ui->previewVideoTimeSlider->setRange(0, 100);

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
    QString filename = QFileDialog::getOpenFileName(this, tr("Select video file"), "", tr("MP4 files (*.mp4)"));
    if (!filename.isEmpty()) {
        videoPreviewWidget->setVideoFile(filename);

        ui->pauseButton->setEnabled(false);
        positionUpdateTimer->start(1000);
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




