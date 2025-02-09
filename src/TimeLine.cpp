#include "../include/TimeLine.h"
#include "../include/VideoPerview.h"
#include "../include/CutRightOperation.h"
#include "../include/CutLeftOperation.h"

TimeLine::TimeLine(QWidget *parent)
        : QWidget(parent),
          draggingVideo(nullptr),
          scaleFactor(10.0),
          currentStateIndex(-1)
{
    setAcceptDrops(true);
    lines = { 0, 60, 120, 180};

    setupShortcuts();
    setupIndicator();
    saveState();

    connect(indicator, &Indicator::requestCurrentVideo, this, [this]() {
        const VideoData *currentVideo = getCurrentIndicatorVideo();
        if (currentVideo) {
            qDebug() << "Current video under indicator:" << currentVideo->getTitle() << "at position:"
                     << currentVideo->getRect().x() << indicator->getPosition().x();
            qDebug() << "Time per unit: " << getTimePerUnit();
            qDebug() << "Time from start: " << calculateTimeFromVideoStart(*currentVideo, indicator->getPosition().x());
        } else {
            qDebug() << "No video under indicator.";
        }
        for (auto video: videoList) {
            qDebug() << "Video:" << video.getTitle() << "Start:" << video.getStartTime() << "End:" << video.getEndTime();
        }
    });

    connect(indicator, &Indicator::cutRight, this, [this]() {
        const VideoData *currentVideo = getCurrentIndicatorVideo();
        if (currentVideo) {
            qDebug() << "Cutting RIGHT video at indicator position.";
            trimVideoAtIndicator();
        }
    });

    connect(indicator, &Indicator::split, this, [this]() {
        const VideoData *currentVideo = getCurrentIndicatorVideo();
        if (currentVideo) {
            qDebug() << "Splitting video at indicator position.";
            splitVideoAtIndicator();
        }
    });

    connect(indicator, &Indicator::cutLeft, this, [this]() {
        const VideoData *currentVideo = getCurrentIndicatorVideo();
        if (currentVideo) {
            qDebug() << "Cutting LEFT video at indicator position.";
            cutLeftVideoAtIndicator();
        }
    });
}



void TimeLine::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    drawSectionBackgrounds(painter);
    drawContentLabels(painter);
    drawLines(painter);
    drawVideos(painter);
    drawTextOnVideos(painter);

    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(indicator->x(), indicator->y() + indicator->height(),
                     indicator->x(), height());
}

void TimeLine::drawVideos(QPainter &painter) {
    for (const VideoData &video: videoList) {
        QRect videoRect = video.getRect();
        painter.setBrush(QBrush(video.getColor()));
        painter.drawRect(videoRect);
    }
}

void TimeLine::drawTextOnVideos(QPainter &painter) {
    painter.setPen(QPen(Qt::black));
    for (const VideoData &video: videoList) {
        QRect videoRect = video.getRect();
        painter.drawText(videoRect, Qt::AlignCenter, video.getTitle());
    }
}

void TimeLine::drawLines(QPainter &painter) {
    painter.setPen(QPen(Qt::black, 2));
    for (int line: lines) {
        painter.drawLine(0, line, width(), line);
    }
}

void TimeLine::dropEvent(QDropEvent *event) {
    qDebug() << "Drop event";
    if (!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        qDebug() << "Invalid mime format";
        event->ignore();
        return;
    }

    VideoData video = extractVideoData(event->mimeData());
    if (video.getFilePath().isEmpty()) {
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

    for (const VideoData &existingVideo: videoList) {
        if (existingVideo.getRect().intersects(rect)) {
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

VideoData TimeLine::extractVideoData(const QMimeData *mimeData) {
    QByteArray itemData = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(itemData);
    QString filePath, duration, title, format;

    while (!stream.atEnd()) {
        int row, col;
        QMap<int, QVariant> valueMap;
        stream >> row >> col >> valueMap;

        if (col == 0) {
            filePath = valueMap.value(Qt::UserRole).toString();
            title = QFileInfo(filePath).baseName();
        }
        else if (col == 1) duration = valueMap.value(Qt::DisplayRole).toString();
        else if (col == 2) format = valueMap.value(Qt::DisplayRole).toString();
    }

    return VideoData(title, duration, filePath, format, QRect());
}

int TimeLine::calculateVideoWidth(const QString &duration) {
    int totalSeconds = QTime::fromString(duration, "mm:ss").msecsSinceStartOfDay() / 1000;
    return totalSeconds;
}

void TimeLine::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->accept();
    } else {
        event->ignore();
    }
}

void TimeLine::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void TimeLine::mousePressEvent(QMouseEvent *event) {
    for (VideoData &video: videoList) {
        if (video.getRect().contains(event->pos())) {
            draggingVideo = &video;
            dragStartPos = event->pos() - video.getRect().topLeft();

            qDebug() << "Started dragging:" << video.getTitle();
            break;
        }
    }
}

void TimeLine::mouseMoveEvent(QMouseEvent *event) {
    if (draggingVideo) {
        moveDraggingVideo(event->pos());
        update();
    }
}

void TimeLine::moveDraggingVideo(const QPoint &pos) {
    QRect rect = draggingVideo->getRect();
    QPoint newPosition = pos - dragStartPos;

    int adjustedX = qBound(60, newPosition.x(), width() - rect.width());
    int adjustedY = qBound(lines[0], findNearestLine(newPosition.y()), lines.last() - rect.height());

    rect.moveTopLeft(QPoint(adjustedX, adjustedY));

    for (const VideoData &video: videoList) {
        if (video.getRect() != draggingVideo->getRect() && video.getRect().intersects(rect)) {
            return;
        }
    }

    draggingVideo->setRect(rect);
    qDebug() << "Dragging " << draggingVideo->getTitle() << " to position: " << rect;
}

void TimeLine::mouseReleaseEvent(QMouseEvent *event) {
    if (draggingVideo) {
        QRect rect = draggingVideo->getRect();

        rect.moveTopLeft(QPoint(rect.topLeft().x(), findNearestLine(rect.topLeft().y())));
        draggingVideo->setRect(rect);

        updateVideoPositions();
        draggingVideo = nullptr;
        qDebug() << "Film released at position: " << rect;
        saveState();
        update();
    }
    qDebug() << getVideoDurationTime();
}

void TimeLine::wheelEvent(QWheelEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {
        scaleVideos(event->angleDelta().y() > 0 ? 1.1 : 0.9);
        event->accept();
        update();
    } else {
        QWidget::wheelEvent(event);
    }
}

void TimeLine::scaleVideos(double factor) {
    scaleFactor *= factor;
    constexpr int MIN_WIDTH = 10;
    for (auto &video: videoList) {
        int newWidth = static_cast<int>(video.getRect().width() * factor);
        video.getRect().setWidth(qMax(newWidth, MIN_WIDTH));
    }
}

int TimeLine::findNearestLine(int y) {
    if (y < lines[1]) {
        return lines[0];
    } else if (y < lines[2]) {
        return lines[1];
    } else {
        return lines[2];
    }
}

void TimeLine::drawContentLabels(QPainter &painter) {
    painter.setPen(QPen(Qt::black));
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);

    QRect videoLabelRect(10, lines[1] - 50, 100, 30);
    painter.drawText(videoLabelRect, Qt::AlignLeft | Qt::AlignVCenter, "Video");

    QRect textLabelRect(10, lines[2] - 50, 100, 30);
    painter.drawText(textLabelRect, Qt::AlignLeft | Qt::AlignVCenter, "Text");

    QRect imageLabelRect(10, lines[3] - 50, 100, 30);
    painter.drawText(imageLabelRect, Qt::AlignLeft | Qt::AlignVCenter, "Image");
}

void TimeLine::updateVideoPositions() {
    std::sort(videoList.begin(), videoList.end(), [](const VideoData &a, const VideoData &b) {
        return a.getRect().left() < b.getRect().left();
    });

    qDebug() << "Sorted videos:";
    for (const VideoData &video: videoList)
        qDebug() << "Film:" << video.getTitle() << "at x=" << video.getRect().left();

    update();
}


void TimeLine::drawSectionBackgrounds(QPainter &painter) {
    painter.fillRect(QRect(0, 0, 60, lines[1]), QColor(230, 240, 255));
    painter.fillRect(QRect(0, lines[1], 60, lines[2] - lines[1]), QColor(230, 255, 230));
    painter.fillRect(QRect(0, lines[2], 60, height() - lines[2]), QColor(255, 240, 230));
}

void TimeLine::saveState() {
    qDebug() << "Saving state. Current state index before save:" << currentStateIndex;

    caretaker.clearMementosAfter(currentStateIndex);
    originator.setState(videoList);
    caretaker.addMemento(originator.saveStateToMemento());
    currentStateIndex++;

    qDebug() << "State saved. Current state index after save:" << currentStateIndex;
}

void TimeLine::undoState() {
    qDebug() << "Attempting undo. Current state index:" << currentStateIndex;

    if (currentStateIndex > 0) {
        currentStateIndex--;

        originator.getStateFromMemento(caretaker.getMemento(currentStateIndex));
        videoList = originator.getState();

        if (cutInProgress) {
            copiedVideo.reset();
            cutInProgress = false;
        }

        qDebug() << "Undo successful. New state index:" << currentStateIndex;
        update();
    } else {
        qDebug() << "Undo not possible. No more states to revert.";
    }
}

void TimeLine::setupShortcuts()
{
    QShortcut *undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(undoShortcut, &QShortcut::activated, this, &TimeLine::undoState);

    QShortcut *copyShortcut = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(copyShortcut, &QShortcut::activated, this, &TimeLine::copySelectedVideo);

    QShortcut *pasteShortcut = new QShortcut(QKeySequence("Ctrl+V"), this);
    connect(pasteShortcut, &QShortcut::activated, this, &TimeLine::pasteCopiedVideo);

    QShortcut *cutShortcut = new QShortcut(QKeySequence("Ctrl+X"), this);
    connect(cutShortcut, &QShortcut::activated, this, &TimeLine::cutSelectedVideo);

    QShortcut *deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(deleteShortcut, &QShortcut::activated, this, &TimeLine::deleteSelectedVideo);
}

void TimeLine::copySelectedVideo() {
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    for (const VideoData &video : videoList) {
        if (video.getRect().contains(cursorPos)) {
            copiedVideo = video;
            cutInProgress = false;
            qDebug() << "Video copied: " << copiedVideo->getTitle();
            return;
        }
    }
    qDebug() << "No video under cursor to copy.";
}

void TimeLine::pasteCopiedVideo() {
    if (copiedVideo.has_value()) {
        VideoData newVideo = copiedVideo.value();
        QRect newRect = newVideo.getRect();

        QPoint newPosition = mapFromGlobal(QCursor::pos());

        int snappedY = findNearestLine(newPosition.y());
        newRect.moveTo(newPosition.x(), snappedY);
        newVideo.setRect(newRect);

        videoList.push_back(newVideo);

        saveState();
        update();

        if (cutInProgress) {
            copiedVideo.reset();
            cutInProgress = false;
        }
    }
}

void TimeLine::cutSelectedVideo() {
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    for (auto it = videoList.begin(); it != videoList.end(); ++it) {
        if (it->getRect().contains(cursorPos)) {
            cutVideo(it);
            saveState();
            update();
            return;
        }
    }
    qDebug() << "No video under cursor to cut.";
}

void TimeLine::deleteSelectedVideo() {
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    for (auto it = videoList.begin(); it != videoList.end(); ++it) {
        if (it->getRect().contains(cursorPos)) {
            qDebug() << "Video deleted: " << it->getTitle();
            videoList.erase(it);
            saveState();
            update();
            return;
        }
    }
    qDebug() << "No video under cursor to delete.";
}

void TimeLine::cutVideo(QList<VideoData>::iterator i) {
    copiedVideo = *i;
    cutInProgress = true;
    videoList.erase(i);
}

void TimeLine::contextMenuEvent(QContextMenuEvent *event) {
    QMenu contextMenu(this);
    QAction *selectedAction = nullptr;

    for (auto i = videoList.begin(); i != videoList.end(); i++) {
        if (i->getRect().contains(event->pos())) {
            QAction *copyAction = contextMenu.addAction("Copy");
            copyAction->setShortcut(QKeySequence("Ctrl+C"));
            copyAction->setShortcutVisibleInContextMenu(true);

            QAction *cutAction = contextMenu.addAction("Cut");
            cutAction->setShortcut(QKeySequence("Ctrl+X"));
            cutAction->setShortcutVisibleInContextMenu(true);

            QAction *deleteAction = contextMenu.addAction("Delete");
            deleteAction->setShortcut(QKeySequence("Del"));
            deleteAction->setShortcutVisibleInContextMenu(true);

            selectedAction = contextMenu.exec(event->globalPos());

            if (selectedAction == copyAction) {
                copiedVideo = *i;
                qDebug() << "Video copied: " << copiedVideo->getTitle();
            } else if (selectedAction == cutAction) {
                cutVideo(i);
                saveState();
                update();
            } else if (selectedAction == deleteAction) {
                videoList.erase(i);
                saveState();
                update();
            }
            return;
        }
    }

    if (!selectedAction) {
        selectedAction = contextMenu.exec(event->globalPos());
    }

   if (selectedAction && copiedVideo.has_value()) {
        QAction *pasteAction = contextMenu.addAction("Paste");
        pasteAction->setShortcut(QKeySequence("Ctrl+V"));
        pasteAction->setShortcutVisibleInContextMenu(true);

        if (selectedAction == pasteAction) {
            pasteCopiedVideo();
        }
    }
}

void TimeLine::removeVideoObjects(const QString &filePath) {
    for (auto it = videoList.begin(); it != videoList.end(); ) {
        if (it->getFilePath() == filePath) {
            it = videoList.erase(it);
        } else {
            ++it;
        }
    }
    saveState();
    update();
}

QList<VideoData> TimeLine::getVideoList() const {
    return this->videoList;
}

void TimeLine::setupIndicator() {
    indicator = new Indicator(this);
    indicator->move(60, 0);
}

const VideoData *TimeLine::getCurrentIndicatorVideo() const {
    int indicatorPos = indicator->getPosition().x();
    for (const VideoData &video: videoList) {
        QRect videoRect = video.getRect();
        if (videoRect.left() <= indicatorPos && videoRect.right() >= indicatorPos) {
            return &video;
        }
    }
    return nullptr;
}

const int TimeLine::getCurrentVideoIndexIndicator() const {
    int indicatorPos = indicator->getPosition().x();
    for (int i = 0; i < videoList.size(); i ++) {
        QRect videoRect = videoList[i].getRect();
        if (videoRect.left() <= indicatorPos && videoRect.right() >= indicatorPos) {
            return i;
        }
    }
    return 0;
}

void TimeLine::setVideoList(const QList<VideoData>& newVideoList)
{
    videoList = newVideoList;
    updateVideoPositions();
    update();
}

QTime TimeLine::getVideoDurationTime() {
    int totalMilliseconds = 0;
    for (const VideoData &video: videoList) {
        QTime duration = QTime::fromString(video.getDuration(), "mm:ss");
        totalMilliseconds += QTime(0, 0).msecsTo(duration);
    }
    return QTime(0, 0).addMSecs(totalMilliseconds);
}

int TimeLine::getVideoDurationWidth() {
    int totalWidth = 0;
    for (const VideoData &video: videoList) {
        totalWidth += video.getRect().width();
    }
    return totalWidth;
}

qreal TimeLine::getTimePerUnit() {
    int totalMilliseconds = QTime(0, 0).msecsTo(getVideoDurationTime());
    int totalWidth = getVideoDurationWidth();
    if (totalWidth == 0) {
        return 0.0;
    }
    return static_cast<qreal>(totalMilliseconds) / totalWidth;
}

qreal TimeLine::calculateTimeFromVideoStart(const VideoData &video, int x) {
    int videoStart = video.getRect().left();
    int distance = x - videoStart;
    return distance * getTimePerUnit();
}

void TimeLine::cutVideoAtIndicator(bool cutLeft) {
    VideoData* currentVideo = const_cast<VideoData*>(getCurrentIndicatorVideo());
    if (currentVideo) {
        qreal cutTime = calculateTimeFromVideoStart(*currentVideo, indicator->getPosition().x());
        QString filePath = currentVideo->getFilePath();

        if (cutLeft) {
            currentVideo->addOperation(new CutLeftOperation(filePath, cutTime));
            currentVideo->setStartTime(cutTime);
        } else {
            currentVideo->addOperation(new CutRightOperation(filePath, cutTime));
            currentVideo->setEndTime(cutTime);
        }

        int indicatorPos = indicator->getPosition().x();
        QRect videoRect = currentVideo->getRect();
        if (indicatorPos > videoRect.left() && indicatorPos < videoRect.right()) {
            if (cutLeft) {
                int newWidth = videoRect.right() - indicatorPos;
                videoRect.setLeft(indicatorPos);
                videoRect.setWidth(newWidth);
            } else {
                int newWidth = indicatorPos - videoRect.left();
                videoRect.setWidth(newWidth);
                videoRect.setRight(indicatorPos);
            }
            currentVideo->setRect(videoRect);
            update();
        }
    }

    emit resetVideoPlayer();

    for (VideoData& video : videoList) {
        if (video.getOperations().length() > 0) {
            qDebug() << (cutLeft ? "CUTTING" : "Trimming") << "video:" << video.getTitle();
        }
    }
}

void TimeLine::cutLeftVideoAtIndicator() {
    cutVideoAtIndicator(true);
}

void TimeLine::trimVideoAtIndicator() {
    cutVideoAtIndicator(false);
}

void TimeLine::splitVideoAtIndicator() {
    VideoData* currentVideo = const_cast<VideoData*>(getCurrentIndicatorVideo());
    if (currentVideo) {
        double splitTime = calculateTimeFromVideoStart(*currentVideo, indicator->getPosition().x());

        QString filePath = currentVideo->getFilePath();

        QRect videoRect = currentVideo->getRect();
        QRect newRect1(videoRect.topLeft(), QPoint(indicator->getPosition().x(), videoRect.bottom()));
        QRect newRect2(QPoint(indicator->getPosition().x() + 1, videoRect.top()), videoRect.bottomRight());

        currentVideo->addOperation(new CutRightOperation(filePath, splitTime));
        currentVideo->setStartTime(currentVideo->getStartTime());

        VideoData newVideo = VideoData(currentVideo->getTitle(), currentVideo->getDuration(), currentVideo->getFilePath(), currentVideo->getExtension(), newRect2);
        newVideo.addOperation(new CutLeftOperation(filePath, splitTime));
        newVideo.setStartTime(splitTime);
        newVideo.setEndTime(currentVideo->getEndTime());
        currentVideo->setEndTime(splitTime);


        currentVideo->setRect(newRect1);
        videoList.append(newVideo);
        update();
    }

    emit resetVideoPlayer();
}

