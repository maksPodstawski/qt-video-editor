#include "../include/TimeLine.h"
#include "../include/VideoPerview.h"

TimelineWidget::TimelineWidget(QWidget *parent)
        : QGraphicsView(parent),
        scene(new QGraphicsScene(this)),
        draggedItem(nullptr)
{
    setScene(scene);
    setAcceptDrops(true);

    setBackgroundBrush(QBrush(QColor(40, 44, 52)));

    QRectF sceneRect(-100, -50, 5000, 200);
    scene->setSceneRect(sceneRect);
    sceneStartY = sceneRect.top();

    initializeScene();
    initializeGrid();

}

void TimelineWidget::initializeScene()
{
    QGraphicsRectItem *timelinePath = new QGraphicsRectItem(scene->sceneRect());
    timelinePath->setBrush(QBrush(QColor(50, 55, 65)));
    timelinePath->setPen(QPen(Qt::NoPen));
    scene->addItem(timelinePath);
}

void TimelineWidget::initializeGrid()
{
    QPen gridPen(QColor(60, 65, 75));
    QRectF sceneRect = scene->sceneRect();

    for (int y = sceneRect.top(); y < sceneRect.bottom(); y += LINE_HEIGHT) {
        scene->addLine(sceneRect.left(), y, sceneRect.right(), y, gridPen);
    }
}


void TimelineWidget::dropEvent(QDropEvent *event)
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
    while (!stream.atEnd()) {
        int row, col;
        QMap<int, QVariant> valueMap;
        stream >> row >> col >> valueMap;

        if (col == 0) {
            filePath = valueMap.value(Qt::UserRole).toString();
        }
        else if (col == 1) {
            duration = valueMap.value(Qt::DisplayRole).toString();
        }
        else if (col == 2) {
            format = valueMap.value(Qt::DisplayRole).toString();
        }
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

    QPointF scenePos = mapToScene(event->position().toPoint());
    VideoData video(title, duration, filePath, format);
    addVideoItem(video, scenePos);
    filmsList.append(video);

    qDebug() << "Added video:";
    qDebug() << "Title:" << video.getTitle();
    qDebug() << "FilePath:" << video.getFilePath();
    qDebug() << "Duration:" << video.getDuration();
    qDebug() << "Format:" << video.getExtension();

    updateFilmsList();
    event->accept();
}

void TimelineWidget::addVideoItem(const VideoData &video, const QPointF &pos)
{
    createVideoItem(video, pos);
}

void TimelineWidget::createVideoItem(const VideoData &video, const QPointF &pos)
{
    int totalSeconds = QTime::fromString(video.getDuration(), "mm:ss").msecsSinceStartOfDay() / 1000;
    const int rectWidth = 15 * totalSeconds;
    const int rectHeight = 30;

    auto *videoItem = new QGraphicsRectItem();
    videoItem->setRect(0, 0, rectWidth, rectHeight);

    qreal snappedY = snapToNearestLine(pos.y());
    videoItem->setPos(pos.x(), snappedY);
    videoItem->setBrush(QBrush(generateRandomColor()));
    videoItem->setData(Qt::UserRole, video.getFilePath());

    videoItem->setFlag(QGraphicsItem::ItemIsMovable);

    createTextItemForVideo(video, videoItem);
    scene->addItem(videoItem);
}

void TimelineWidget::createTextItemForVideo(const VideoData &video, QGraphicsRectItem *videoItem)
{
    QFileInfo fileInfo(video.getFilePath());
    auto *textItem = new QGraphicsTextItem(videoItem);

    QFont font = textItem->font();
    font.setPointSize(8);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::black);

    QString fileName = video.getTitle();
    QFontMetrics fm(font);
    int maxWidth = videoItem->rect().width() - 10;
    QString elidedText = fm.elidedText(fileName, Qt::ElideMiddle, maxWidth);

    textItem->setPlainText(elidedText);

    QRectF textRect = textItem->boundingRect();
    qreal textX = (videoItem->rect().width() - textRect.width()) / 2;
    qreal textY = (videoItem->rect().height() - textRect.height()) / 2;
    textItem->setPos(textX, textY);
}

QColor TimelineWidget::generateRandomColor() {
    return QColor::fromRgb(rand() % 156 + 100, rand() % 156 + 100, rand() % 156 + 100);
}

void TimelineWidget::updatePositionForDraggedItem(const QPointF &scenePos)
{
    if (draggedItem) {
        qreal snappedY = snapToNearestLine(scenePos.y() - dragOffset.y());
        qreal newX = scenePos.x() - dragOffset.x();
        draggedItem->setPos(newX, snappedY);
    }
}

void TimelineWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void TimelineWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        if(event->angleDelta().y() > 0) {
            scale(1.1,1);
        }
        else {
            scale(0.9, 1);
        }
        event->accept();
    }
    else {
        QGraphicsView::wheelEvent(event);
    }
}

int TimelineWidget::snapToNearestLine(qreal yPos)
{
    qreal snapped = sceneStartY + qRound((yPos - sceneStartY) / LINE_HEIGHT) * LINE_HEIGHT;
    qDebug() << "snapToNearestLine: yPos =" << yPos << ", snappedY =" << snapped;
    return snapped;
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (draggedItem && event->buttons() & Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        updatePositionForDraggedItem(scenePos);
        event->accept();
    }
    else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void TimelineWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *item = scene->itemAt(scenePos, QTransform());

    while (item) {
        if (item->type() == QGraphicsRectItem::Type) {
            draggedItem = static_cast<QGraphicsRectItem *>(item);
            dragOffset = scenePos - draggedItem->pos();
            break;
        }
        item = item->parentItem();
    }
    QGraphicsView::mousePressEvent(event);
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (draggedItem) {
        QPointF scenePos = draggedItem->pos();
        qreal snappedY = snapToNearestLine(scenePos.y());

        draggedItem->setPos(scenePos.x(), snappedY);
        draggedItem = nullptr;

        updateFilmsList();
        event->accept();
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void TimelineWidget::updateFilmsList()
{
    QMap<qreal, VideoData> positionToVideoMap;

    for(QGraphicsItem *item : scene->items())
    {
        if(item->type() == QGraphicsRectItem::Type)
        {
            QString filePath = item->data(Qt::UserRole).toString();
            if(!filePath.isEmpty())
            {
                for (const VideoData &video : filmsList) {
                    if (video.getFilePath() == filePath) {
                        positionToVideoMap.insert(item->pos().x(), video);
                        break;
                    }
                }
            }
        }
    }

    filmsList.clear();

    for(const VideoData &video : positionToVideoMap.values()) {
        filmsList.append(video);
    }

    QList<QString> titleList;
    for (const VideoData &video : filmsList) {
        titleList.append(video.getTitle());
    }

    qDebug() << "Updated films list (sorted by x):" << titleList;
}


