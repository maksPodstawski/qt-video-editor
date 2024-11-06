#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include "videotablewidget.h"
#include "Video.h"

VideoTableWidget::VideoTableWidget(QWidget *parent)
    : QWidget{parent},
    tableWidget(new QTableWidget(this))
{
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Title" << "Duration" << "Path");

    tableWidget->setDragEnabled(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableWidget->setDragDropMode(QAbstractItemView::DragOnly);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
}

void VideoTableWidget::updateTable(const QList<QString> &videoFiles)
{
    tableWidget->setRowCount(videoFiles.size());

    for(int i = 0; i < videoFiles.size(); ++i) {
        QFileInfo fileInfo(videoFiles[i]);
        QString title = fileInfo.fileName();
        QString durationText = getDurationText(videoFiles[i]);
        QString filePath = videoFiles[i];

        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(Qt::UserRole, filePath);
        tableWidget->setItem(i, 0, titleItem);

        QTableWidgetItem *durationItem = new QTableWidgetItem(durationText);
        tableWidget->setItem(i, 1, durationItem);

        QTableWidgetItem *pathItem = new QTableWidgetItem(filePath);
        tableWidget->setItem(i, 2, pathItem);
    }
}



QString VideoTableWidget::getDurationText(const QString &filePath)
{
    QMediaPlayer mediaPlayer;
    mediaPlayer.setSource(QUrl::fromLocalFile(filePath));

    QEventLoop loop;
    connect(&mediaPlayer, &QMediaPlayer::mediaStatusChanged, &loop, [&](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::EndOfMedia) {
            loop.quit();
        }
    });
    loop.exec();

    int duration = mediaPlayer.duration();
    return QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss");
}

void VideoTableWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void VideoTableWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPosition).manhattanLength()
        < QApplication::startDragDistance())
        return;

    QModelIndex index = tableWidget->indexAt(dragStartPosition);
    if (!index.isValid())
        return;

    startDrag(dragStartPosition);
}

void VideoTableWidget::startDrag(const QPoint &pos) {
    QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid())
        return;

    QByteArray itemData = serializeRow(index.row());

    qDebug() << "Dragging row:" << index.row() << "with data:" << itemData;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction);
}

QByteArray VideoTableWidget::serializeRow(int row) const {


    int currentRow = row;

    QTableWidgetItem *pathItem = tableWidget->item(currentRow, 3);

    QTableWidgetItem *durationItem = tableWidget->item(currentRow, 1);
    QString duration;
    if (durationItem) {
        duration = durationItem->text();
        qDebug() << "Duration found:" << duration;
    } else {
        qDebug() << "No duration item in column 1";
    }

    QString filePath = pathItem->data(Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        qDebug() << "No file path stored in item";
    }

    qDebug() << "Preparing drag data:";
    qDebug() << "Duration:" << duration;
    qDebug() << "FilePath:" << filePath;

    QByteArray itemData;
    QDataStream stream(&itemData, QIODevice::WriteOnly);
    stream << currentRow << pathItem->text() << duration << filePath;

    return itemData;
}


