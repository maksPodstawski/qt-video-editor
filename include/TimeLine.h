#ifndef TIMELINE_H
#define TIMELINE_H

#include "VideoData.h"
#include "Memento.h"
#include "Originator.h"
#include "Caretaker.h"
#include "Indicator.h"
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
#include <QSlider>

class TimeLine : public QWidget
{
    Q_OBJECT
public:
    explicit TimeLine(QWidget *parent = nullptr);
    QList<VideoData> getVideoList() const;

    void saveState();
    void undoState();
    Indicator *indicator;
    double getTimePerUnit();
    const VideoData* getCurrentIndicatorVideo() const;
    const int getCurrentVideoIndexIndicator() const;

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
    void trimVideoAtIndicator();
    void cutLeftVideoAtIndicator();
    void splitVideoAtIndicator();

    Caretaker caretaker;
    Originator originator;
    int currentStateIndex;
    QShortcut *undoShortcut;


    std::optional<VideoData> copiedVideo;
    std::optional<VideoData> cutVideo;
    bool cutInProgress = false;

    void setupIndicator();



    QTime getVideoDurationTime();
    int getVideoDurationWidth();


    double calculateTimeFromVideoStart(const VideoData &video, int x);

};

#endif // TIMELINE_H
