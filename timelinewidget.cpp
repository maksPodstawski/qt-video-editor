#include "timelinewidget.h"
#include "videotablewidget.h"

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
    qDebug() << "Mime formats:" << event->mimeData()->formats();

    if (!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        qDebug() << "Invalid mime format";
        event->ignore();
        return;
    }

    QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(itemData);

    QString filePath, duration;
    while (!stream.atEnd()) {
        int row, col;
        QMap<int, QVariant> valueMap;
        stream >> row >> col >> valueMap;

        qDebug() << "Reading data for column:" << col;
        qDebug() << "ValueMap contents:" << valueMap;

        if (col == 0) {
            filePath = valueMap.value(Qt::UserRole).toString();
        } else if (col == 1) {
            duration = valueMap.value(Qt::DisplayRole).toString();
        }
    }

    qDebug() << "Extracted data:";
    qDebug() << "FilePath:" << filePath;
    qDebug() << "Duration:" << duration;

    if (filePath.isEmpty()) {
        qDebug() << "File path is empty";
        event->ignore();
        return;
    }

    if (duration.isEmpty()) {
        qDebug() << "Duration is empty, using default value";
        duration = "00:00";
    }

    QPointF scenePos = mapToScene(event->position().toPoint());

    addVideoItem(filePath, duration, scenePos);

    filmsList.append(filePath);
    qDebug() << "Films list: " << filmsList;

    updateFilmsList();
    event->accept();
}

void TimelineWidget::addVideoItem(const QString &filePath, const QString &duration, const QPointF &pos)
{
    int totalSeconds = QTime::fromString(duration, "mm:ss").msecsSinceStartOfDay() / 1000;

    const int rectWidth = 15 * totalSeconds;
    const int rectHeight = 30;

    auto *videoItem = new QGraphicsRectItem();
    videoItem->setRect(0, 0, rectWidth, rectHeight);

    qreal snappedY = snapToNearestLine(pos.y());
    videoItem->setPos(pos.x(), snappedY);
    videoItem->setBrush(QBrush(Qt::lightGray));
    videoItem->setData(Qt::UserRole, filePath);

    videoItem->setFlag(QGraphicsItem::ItemIsMovable);

    QFileInfo fileInfo(filePath);
    auto *textItem = new QGraphicsTextItem(videoItem);

    QFont font = textItem->font();
    font.setPointSize(8);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::black);

    QString fileName = fileInfo.fileName();
    QFontMetrics fm(font);
    int maxWidth = rectWidth - 10;
    QString elidedText = fm.elidedText(fileName, Qt::ElideMiddle, maxWidth);

    textItem->setPlainText(elidedText);

    QRectF textRect = textItem->boundingRect();
    qreal textX = (rectWidth - textRect.width()) / 2;
    qreal textY = (rectHeight - textRect.height()) / 2;
    textItem->setPos(textX, textY);

    scene->addItem(videoItem);

    qDebug() << "Added video item for file:" << filePath
             << "at position:" << pos;
}


void TimelineWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << event->mimeData()->formats();
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->accept();
    } else {
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

        qreal snappedY = snapToNearestLine(scenePos.y() - dragOffset.y());
        qreal newX = scenePos.x() - dragOffset.x();

        draggedItem->setPos(newX, snappedY);
        qDebug() << "Moved item to:" << draggedItem->pos();
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
    QMap<qreal, QString> positionToFileMap;

    for(QGraphicsItem *item : scene->items())
    {
        if(item->type() == QGraphicsRectItem::Type)
        {
            QString filePath = item->data(Qt::UserRole).toString();
            if(!filePath.isEmpty())
            {
                positionToFileMap.insert(item->pos().x(), filePath);
            }
        }
    }

    filmsList = positionToFileMap.values();
    qDebug() << "Updated films list (sorted by x):" << filmsList;
}