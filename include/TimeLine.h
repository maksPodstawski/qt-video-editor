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


class TimeLine : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TimeLine(QWidget *parent = nullptr);
    QList<VideoData> getFilmsList() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *scene;
    QList<VideoData> filmsList;
    QGraphicsItem *draggedItem;
    QPointF dragOffset;
    qreal sceneStartY;
    const int LINE_HEIGHT = 30;
    double timelineScale = 1.0;
    
    void addVideoItem(const VideoData &video, const QPointF &pos);
    void initializeScene();
    void initializeGrid();
    void updateFilmsList();
    void createTextItemForVideo(const VideoData &video, QGraphicsRectItem *videoItem);
    void createVideoItem(const VideoData &video, const QPointF &pos);
    void updatePositionForDraggedItem(const QPointF &scenePos);
    int snapToNearestLine(qreal yPos);
    QColor generateRandomColor();

};

#endif // TIMELINE_H
