#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QGraphicsTextItem>
#include <QFileInfo>
#include <QDrag>
#include <QApplication>
#include "Video.h"

class TimelineWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);


protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsItem *draggedItem;
    QPoint dragStartPos;

    void addVideoItem(const QString &filePath, const QString &duration, const QPointF &pos);


};

#endif // TIMELINEWIDGET_H
