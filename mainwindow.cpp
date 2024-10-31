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

    connect(positionUpdateTimer, &QTimer::timeout, this, [this]() {
        if (videoPreviewWidget->getDuration() > 0) {
            qint64 currentPosition = videoPreviewWidget->getPosition();
            qint64 duration = videoPreviewWidget->getDuration();
            int sliderPosition = (currentPosition * 100) / duration;

            ui->previewVideoTimeSlider->setValue(sliderPosition);

        }
    });

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select video file"), "", tr("MP4 files (*.mp4)"));
    if (!filename.isEmpty()) {
        videoPreviewWidget->setVideoFile(filename);
        videoPreviewWidget->play();

        positionUpdateTimer->start(1000);
    }
}
