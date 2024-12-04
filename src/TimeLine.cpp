#include "../include/TimeLine.h"
#include "../include/VideoPerview.h"

TimeLine::TimeLine(QWidget *parent)
        : QWidget(parent),
        draggingVideo(nullptr)
{
    setAcceptDrops(true);
    lines = {50, 80, 110, 140, 170};
    draggingVideo = nullptr;
}

void TimeLine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawLines(painter);

    for (const VideoData &video : videoList)
    {
        QRect videoRect = video.getRect();
        painter.setBrush(QBrush(video.getColor()));
        painter.drawRect(videoRect);
    }

    drawTextOnVideos(painter);
}

void TimeLine::drawTextOnVideos(QPainter &painter)
{
    painter.setPen(QPen(Qt::black));

    for (const VideoData &video : videoList) {
        QRect videoRect = video.getRect();
        painter.drawText(videoRect, Qt::AlignCenter, video.getTitle());
    }
}

void TimeLine::drawLines(QPainter &painter)
{
    painter.setPen(QPen(Qt::black, 2));
    for(int line : lines)
        painter.drawLine(0, line, width(), line);
}

void TimeLine::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop event";
    if (!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        qDebug() << "Invalid mime format";
        event->ignore();
        return;
    }

    QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
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
        else if (col == 3) {
            QFileInfo fileInfo(valueMap.value(Qt::DisplayRole).toString());
            title = fileInfo.baseName();
        }
    }

    if (filePath.isEmpty()) {
        qDebug() << "File path is empty";
        event->ignore();
        return;
    }

    QPoint scenePos = event->position().toPoint();
    qDebug() << "scenePos.x():" << scenePos.x() << "scenePos.y():" << scenePos.y();
    int nearestLine = findNearestLine(scenePos.y());
    qDebug() << "nearestLine:" << nearestLine;

    QRect rect(scenePos.x() + 20, nearestLine, 100, LINE_HEIGHT);
    VideoData video(title, duration, filePath, format, rect);

    video.getRect().moveTopLeft(QPoint(100, nearestLine));
    videoList.append(video);

    qDebug() << "Added video:";
    qDebug() << "Title:" << video.getTitle();
    qDebug() << "FilePath:" << video.getFilePath();
    qDebug() << "Duration:" << video.getDuration();
    qDebug() << "Format:" << video.getExtension();

    update();
    event->accept();
}

void TimeLine::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->accept();
    }
    else {
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
        QRect rect = draggingVideo->getRect();
        QPoint newPosition = event->pos() - dragStartPos;

        int minX = 0;
        int maxX = width() - rect.width();
        int adjustedX = qBound(minX, newPosition.x(), maxX);

        int nearestLine = findNearestLine(newPosition.y());
        int minY = lines[0];
        int maxY = lines[lines.size() - 1] - rect.height();
        int adjustedY = qBound(minY, nearestLine, maxY);


        rect.moveTopLeft(QPoint(adjustedX, adjustedY));
        draggingVideo->setRect(rect);
        qDebug() << "Dragging " << draggingVideo->getTitle() << " to position: " << rect;

        update();
    }
}

void TimeLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (draggingVideo)
    {
        QRect rect = draggingVideo->getRect();
        int nearestLine = findNearestLine(rect.topLeft().y());

        rect.moveTopLeft(QPoint(rect.topLeft().x(), nearestLine));
        draggingVideo->setRect(rect);

        updateVideoPositions();

        draggingVideo = nullptr;
        qDebug() << "Film released at position: " << rect;

        update();
    }
}

void TimeLine::wheelEvent(QWheelEvent *event)
{
    constexpr int MIN_WIDTH = 10;

    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->angleDelta().y() > 0) scaleFactor = 1.1;
        else scaleFactor = 0.9;

        for (auto &video : videoList)
        {
            int newWidth = static_cast<int>(video.getRect().width() * scaleFactor);

            if (newWidth >= MIN_WIDTH) video.getRect().setWidth(newWidth);
            else video.getRect().setWidth(MIN_WIDTH);
        }

        event->accept();
        update();
    }
    else {
        QWidget::wheelEvent(event);
    }
}

int TimeLine::findNearestLine(int y)
{
    int nearestLine = lines[0];
    int minDistance = qAbs(y - lines[0]);

    for (int i = 1; i < lines.size(); ++i)
    {
        int distance = qAbs(y - lines[i]);

        if (distance < minDistance) {
            minDistance = distance;
            nearestLine = lines[i];
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


