#ifndef TIMELINE_H
#define TIMELINE_H

#include "VideoData.h"
#include "Memento.h"
#include "Originator.h"
#include "Caretaker.h"
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
#include <QShortcut>
#include <QMenu>


class TimeLine : public QWidget
{
    Q_OBJECT
public:
    explicit TimeLine(QWidget *parent = nullptr);
    QList<VideoData> getVideoList() const;

    void saveState();
    void undoState();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;


private:

    QList<int> lines;
    QList<VideoData> videoList;
    double scaleFactor;
    VideoData *draggingVideo;
    QPoint dragStartPos;

    const int LINE_HEIGHT = 30;

    int findNearestLine(int y);
    void drawVideos(QPainter &painter);
    void drawLines(QPainter &painter);
    void drawTextOnVideos(QPainter &painter);
    VideoData extractVideoData(const QMimeData *mimeData);
    int calculateVideoWidth(const QString &duration);
    void moveDraggingVideo(const QPoint &pos);
    void scaleVideos(double factor);
    void updateVideoPositions();
    void setupShortcuts();

    Caretaker caretaker;
    Originator originator;
    int currentStateIndex;
    QShortcut *undoShortcut;


    std::optional<VideoData> copiedVideo;
    std::optional<VideoData> cutVideo;
    bool cutInProgress = false;

};

#endif // TIMELINE_H
