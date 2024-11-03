#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QGraphicsTextItem>
#include <QFileInfo>

class TimelineWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void addVideoItem(const QString &filePath, const QString &duration, const QPointF &pos);

private:
    QGraphicsScene *scene;
};

#endif // TIMELINEWIDGET_H
