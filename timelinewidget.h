#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QGraphicsTextItem>
#include <QFileInfo>
#include <QApplication>


class TimelineWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);

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
    QList<QString> filmsList;
    QGraphicsItem *draggedItem;
    QPointF dragOffset;
    qreal sceneStartY;
    const int LINE_HEIGHT = 30;
    
    void addVideoItem(const QString &filePath, const QString &duration, const QPointF &pos);
    void initializeScene();
    void initializeGrid();
    void updateFilmsList();
    int snapToNearestLine(qreal yPos);
};

#endif // TIMELINEWIDGET_H
