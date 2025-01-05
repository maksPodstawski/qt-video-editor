#include "../include/TimeLine.h"
#include "../include/VideoPerview.h"

TimeLine::TimeLine(QWidget *parent)
        : QWidget(parent),
        draggingVideo(nullptr),
        scaleFactor(1.0),
        currentStateIndex(-1)
{
    setAcceptDrops(true);
    lines = {50, 80, 110, 140, 170};

    setupShortcuts();
    saveState();
}

void TimeLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawLines(painter);
    drawVideos(painter);
    drawTextOnVideos(painter);
}

void TimeLine::drawVideos(QPainter &painter)
{
    for (const VideoData &video : videoList)
    {
        QRect videoRect = video.getRect();
        painter.setBrush(QBrush(video.getColor()));
        painter.drawRect(videoRect);
    }
}

void TimeLine::drawTextOnVideos(QPainter &painter)
{
    painter.setPen(QPen(Qt::black));
    for (const VideoData &video : videoList)
    {
        QRect videoRect = video.getRect();
        painter.drawText(videoRect, Qt::AlignCenter, video.getTitle());
    }
}

void TimeLine::drawLines(QPainter &painter)
{
    painter.setPen(QPen(Qt::black, 2));
    for (int line : lines)
    {
        painter.drawLine(0, line, width(), line);
    }
}

void TimeLine::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop event";
    if (!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        qDebug() << "Invalid mime format";
        event->ignore();
        return;
    }

    VideoData video = extractVideoData(event->mimeData());
    if (video.getFilePath().isEmpty())
    {
        event->ignore();
        return;
    }

    QPoint scenePos = event->position().toPoint();
    qDebug() << "scenePos.x():" << scenePos.x() << "scenePos.y():" << scenePos.y();
    int nearestLine = findNearestLine(scenePos.y());
    qDebug() << "nearestLine:" << nearestLine;

    int videoWidth = calculateVideoWidth(video.getDuration()) * scaleFactor;
    QRect rect(scenePos.x() + 20, nearestLine, videoWidth, LINE_HEIGHT);
    video.setRect(rect);

    for (const VideoData &existingVideo : videoList)
    {
        if (existingVideo.getRect().intersects(rect))
        {
            event->ignore();
            return;
        }
    }

    video.setRect(rect);
    videoList.append(video);

    qDebug() << "Added video:";
    qDebug() << "Title:" << video.getTitle();
    qDebug() << "FilePath:" << video.getFilePath();
    qDebug() << "Duration:" << video.getDuration();
    qDebug() << "Format:" << video.getExtension();

    saveState();
    update();
    event->accept();
}

VideoData TimeLine::extractVideoData(const QMimeData *mimeData)
{
    QByteArray itemData = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(itemData);
    QString filePath, duration, title, format;

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int, QVariant> valueMap;
        stream >> row >> col >> valueMap;

        if (col == 0) filePath = valueMap.value(Qt::UserRole).toString();
        else if (col == 1) duration = valueMap.value(Qt::DisplayRole).toString();
        else if (col == 2) format = valueMap.value(Qt::DisplayRole).toString();
        else if (col == 3) title = QFileInfo(valueMap.value(Qt::DisplayRole).toString()).baseName();
    }

    return VideoData(title, duration, filePath, format, QRect());
}

int TimeLine::calculateVideoWidth(const QString &duration)
{
    int totalSeconds = QTime::fromString(duration, "mm:ss").msecsSinceStartOfDay() / 1000;
    return totalSeconds;
}

void TimeLine::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void TimeLine::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void TimeLine::mousePressEvent(QMouseEvent *event)
{
    for (VideoData &video : videoList)
    {
        if (video.getRect().contains(event->pos()))
        {
            draggingVideo = &video;
            dragStartPos = event->pos() - video.getRect().topLeft();

            qDebug() << "Started dragging:" << video.getTitle();
            break;
        }
    }
}

void TimeLine::mouseMoveEvent(QMouseEvent *event)
{
    if (draggingVideo)
    {
        moveDraggingVideo(event->pos());
        update();
    }
}

void TimeLine::moveDraggingVideo(const QPoint &pos)
{
    QRect rect = draggingVideo->getRect();
    QPoint newPosition = pos - dragStartPos;

    int adjustedX = qBound(0, newPosition.x(), width() - rect.width());
    int adjustedY = qBound(lines[0], findNearestLine(newPosition.y()), lines.last() - rect.height());

    rect.moveTopLeft(QPoint(adjustedX, adjustedY));

    for (const VideoData &video : videoList)
    {
        if (video.getRect() != draggingVideo->getRect() && video.getRect().intersects(rect))
        {
            return;
        }
    }

    draggingVideo->setRect(rect);
    qDebug() << "Dragging " << draggingVideo->getTitle() << " to position: " << rect;
}

void TimeLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (draggingVideo)
    {
        QRect rect = draggingVideo->getRect();

        rect.moveTopLeft(QPoint(rect.topLeft().x(), findNearestLine(rect.topLeft().y())));
        draggingVideo->setRect(rect);

        updateVideoPositions();
        draggingVideo = nullptr;
        qDebug() << "Film released at position: " << rect;
        saveState();
        update();
    }
}

void TimeLine::wheelEvent(QWheelEvent *event)
{

    if (event->modifiers() & Qt::ControlModifier)
    {
        scaleVideos(event->angleDelta().y() > 0 ? 1.1 : 0.9);
        event->accept();
        update();
    }
    else
    {
        QWidget::wheelEvent(event);
    }
}

void TimeLine::scaleVideos(double factor)
{
    scaleFactor *= factor;
    constexpr int MIN_WIDTH = 10;
    for (auto &video : videoList)
    {
        int newWidth = static_cast<int>(video.getRect().width() * factor);
        video.getRect().setWidth(qMax(newWidth, MIN_WIDTH));
    }
}

int TimeLine::findNearestLine(int y)
{
    int nearestLine = lines[0];
    int minDistance = qAbs(y - lines[0]);

    for (int line : lines)
    {
        int distance = qAbs(y - line);
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestLine = line;
        }
    }

    return nearestLine;
}

void TimeLine::updateVideoPositions()
{
    std::sort(videoList.begin(), videoList.end(), [](const VideoData &a, const VideoData &b) {
        return a.getRect().left() < b.getRect().left();
    });

    qDebug() << "Sorted videos:";
    for (const VideoData &video : videoList)
        qDebug() << "Film:" << video.getTitle() << "at x=" << video.getRect().left();

    update();
}

void TimeLine::saveState()
{
    qDebug() << "Saving state. Current state index before save:" << currentStateIndex;

    caretaker.clearMementosAfter(currentStateIndex);
    originator.setState(videoList);
    caretaker.addMemento(originator.saveStateToMemento());
    currentStateIndex++;

    qDebug() << "State saved. Current state index after save:" << currentStateIndex;
}

void TimeLine::undoState()
{
    qDebug() << "Attempting undo. Current state index:" << currentStateIndex;

    if (currentStateIndex > 0)
    {
        currentStateIndex--;

        originator.getStateFromMemento(caretaker.getMemento(currentStateIndex));
        videoList = originator.getState();

        if(cutInProgress)
        {
            copiedVideo.reset();
            cutInProgress = false;
        }

        qDebug() << "Undo successful. New state index:" << currentStateIndex;
        update();
    }
    else {
        qDebug() << "Undo not possible. No more states to revert.";
    }
}

void TimeLine::setupShortcuts()
{
    undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(undoShortcut, &QShortcut::activated, this, &TimeLine::undoState);
    
}

void TimeLine::contextMenuEvent(QContextMenuEvent* event)
{
    for(auto i = videoList.begin(); i != videoList.end(); i++)
    {
        if(i->getRect().contains(event->pos()))
        {
            QMenu contextMenu(this);
            QAction *copyAction = contextMenu.addAction("Copy");
            QAction* cutAction = contextMenu.addAction("Cut");
            QAction *deleteAction = contextMenu.addAction("Delete");
            QAction *selectedAction = contextMenu.exec(event->globalPos());

            if (selectedAction == copyAction)
            {
                copiedVideo = *i;
                qDebug() << "Video copied: " << copiedVideo->getTitle();
            }
            else if (selectedAction == cutAction)
            {
                copiedVideo = *i;
                cutInProgress = true;
                videoList.erase(i);
                saveState();
                update();
            }
            else if (selectedAction == deleteAction)
            {
                videoList.erase(i);
                saveState();
                update();
            }
            return;
        }
    }

    if (copiedVideo.has_value())
    {
        QMenu contextMenu(this);
        QAction* pasteAction = contextMenu.addAction("Paste");
        QAction* selectedAction = contextMenu.exec(event->globalPos());

        if (selectedAction == pasteAction)
        {
            VideoData newVideo = copiedVideo.value();
            QRect newRect = newVideo.getRect();

            QPoint newPosition = event->pos();

            int snappedY = findNearestLine(newPosition.y());
            newRect.moveTo(newPosition.x(), snappedY);
            newVideo.setRect(newRect);

            videoList.push_back(newVideo);

            if (cutInProgress)
            {
                copiedVideo.reset();
                cutInProgress = false;
            }

            saveState();
            update();
        }
    }
}



