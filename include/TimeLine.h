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
    explicit TimeLine(QWidget* parent = nullptr);

    void setVideoList(const QList<VideoData>& newVideoList);

    QList<VideoData> getVideoList() const;
    const VideoData* getCurrentIndicatorVideo() const;
    const int getCurrentVideoIndexIndicator() const;
    double getTimePerUnit() const;
    QTime getVideoDurationTime() const;
    int getVideoDurationWidth() const;
    int getTotalVideosDurationTime() const;

    void saveState();
    void undoState();

    Indicator* indicator;

signals:
    void resetVideoPlayer();
    void timelineModified();
    void resetIndicatorPosition();
    void clearMediaPlayer();

public slots:
    void removeVideoObjects(const QString& filePath);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    const int LINE_HEIGHT = 60;
    double scaleFactor;
    int currentStateIndex;
    bool cutInProgress = false;
    QList<int> lines;
    QList<VideoData> videoList;
    VideoData* draggingVideo;
    QPoint dragStartPos;
    Caretaker caretaker;
    Originator originator;
    QShortcut* undoShortcut;
    std::optional<VideoData> copiedVideo;


    void drawSectionBackgrounds(QPainter& painter);
    void drawContentLabels(QPainter& painter);
    void drawTextItems(QPainter& painter);
    void drawVideos(QPainter& painter);
    void drawLines(QPainter& painter);
    void drawTextOnVideos(QPainter& painter);
    void showAddTextDialog(const QPoint& clickPosition);
    void moveDraggingVideo(const QPoint& pos);
    void moveDraggingText(const QPoint& pos);
    void scaleVideos(double factor);
    void updateVideoPositions();
    void setupIndicator();
    void setupShortcuts();
    void copySelectedVideo();
    void pasteCopiedVideo();
    void cutSelectedVideo();
    void deleteSelectedVideo();
    void cutRightVideoAtIndicator();
    void cutLeftVideoAtIndicator();
    void splitVideoAtIndicator();
    void cutVideo(QList<VideoData>::iterator i);
    void cutVideoAtIndicator(bool cutLeft);

    int findNearestLine(int y);
    int calculateVideoWidth(const QString& duration);
    double calculateTimeFromVideoStart(const VideoData& video, int x);
    VideoData extractVideoData(const QMimeData* mimeData);
};

#endif // TIMELINE_H
