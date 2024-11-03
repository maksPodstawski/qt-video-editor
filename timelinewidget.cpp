#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget *parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this))
{
    setScene(scene);
    setAcceptDrops(true);

    setBackgroundBrush(QBrush(QColor(40, 44, 52)));

    QRectF sceneRect(-100, -50, 1000, 200);
    scene->setSceneRect(sceneRect);


    QGraphicsRectItem *timelinePath = new QGraphicsRectItem(sceneRect);
    timelinePath->setBrush(QBrush(QColor(50, 55, 65)));
    timelinePath->setPen(QPen(Qt::NoPen));
    scene->addItem(timelinePath);

    QPen gridPen(QColor(60, 65, 75));
    for (int x = 0; x < sceneRect.width(); x += 100) {
        scene->addLine(x, sceneRect.top(), x, sceneRect.bottom(), gridPen);
    }
}

void TimelineWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->accept();
        event->acceptProposedAction();
        qDebug() << "Accepting drag with model data";
    } else {
        event->ignore();
        qDebug() << "Ignoring drag - wrong format";
    }
}

void TimelineWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "DragMove event";
    event->accept();
    event->acceptProposedAction();
}

void TimelineWidget::addVideoItem(const QString &filePath,const QString &duration, const QPointF &pos)
{
    int totalSeconds = QTime::fromString(duration, "mm:ss").msecsSinceStartOfDay() / 1000;

    const int rectWidth = 15 * totalSeconds;
    const int rectHeight = 30;

    QGraphicsRectItem *videoItem = new QGraphicsRectItem();
    videoItem->setRect(0, 0, rectWidth, rectHeight);
    videoItem->setPos(pos);
    videoItem->setBrush(QBrush(Qt::lightGray));
    videoItem->setData(Qt::UserRole, filePath);


    QFileInfo fileInfo(filePath);
    QGraphicsTextItem *textItem = new QGraphicsTextItem(videoItem);

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

void TimelineWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop event";
    qDebug() << "Mime formats:" << event->mimeData()->formats();

    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream stream(itemData);

        QString filePath;
        QString duration;

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

        if (!filePath.isEmpty()) {
            QPointF scenePos = mapToScene(event->position().toPoint());

            if (duration.isEmpty()) {
                qDebug() << "Duration is empty, using default value";
                duration = "00:00";
            }

            addVideoItem(filePath, duration, scenePos);
            event->accept();
        } else {
            qDebug() << "File path is empty";
            event->ignore();
        }
    } else {
        qDebug() << "Invalid mime format";
        event->ignore();
    }
}

void TimelineWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
