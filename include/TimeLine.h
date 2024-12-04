#ifndef TIMELINE_H
#define TIMELINE_H

#include "VideoData.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QGraphicsTextItem>
#include <QFileInfo>
#include <QApplication>
#include <QColor>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>


class TimeLine : public QWidget
{
    Q_OBJECT
public:
    explicit TimeLine(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    QList<int> lines;
    QList<VideoData> videoList;
    double scaleFactor = 1.0;
    VideoData *draggingVideo;
    QPoint dragStartPos;

    const int LINE_HEIGHT = 30;

    int findNearestLine(int y);
    void drawLines(QPainter &painter);
    void drawTextOnVideos(QPainter &painter);
    void updateVideoPositions();
};

#endif // TIMELINE_H
