#include "../include/VideoTable.h"

VideoTable::VideoTable(QWidget *parent)
    : QWidget{parent},
    tableWidget(new QTableWidget(this))
{
    setUpTableWidget();
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
}

void VideoTable::setUpTableWidget()
{
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"Title", "Duration", "Format", "Path"});
    tableWidget->setDragEnabled(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setDragDropMode(QAbstractItemView::DragOnly);
}

void VideoTable::updateTable(const QList<QString> &mediaFiles)
{
    tableWidget->setRowCount(mediaFiles.size());

    for(int i = 0; i < mediaFiles.size(); ++i)
    {
        const QString &filePath = mediaFiles[i];
        QFileInfo fileInfo(filePath);

        QString title = fileInfo.baseName();
        QString durationText = getDurationText(mediaFiles[i]);
        QString format = getFileFormat(mediaFiles[i]);

        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(Qt::UserRole, filePath);

        tableWidget->setItem(i, 0, titleItem);
        tableWidget->setItem(i, 1, new QTableWidgetItem(durationText));
        tableWidget->setItem(i, 2, new QTableWidgetItem(format));
        tableWidget->setItem(i, 3, new QTableWidgetItem(filePath));

    }
}

QString VideoTable::getDurationText(const QString &filePath)
{
    QMediaPlayer mediaPlayer;
    mediaPlayer.setSource(QUrl::fromLocalFile(filePath));

    QEventLoop loop;
    connect(&mediaPlayer, &QMediaPlayer::mediaStatusChanged, &loop, [&loop](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::EndOfMedia) {
            loop.quit();
        }
    });
    loop.exec();

    int duration = mediaPlayer.duration();
    return QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss");
}

QString VideoTable::getFileFormat(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix().toUpper();
}

void VideoTable::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void VideoTable::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QModelIndex index = tableWidget->indexAt(dragStartPosition);
    if (!index.isValid())
        return;

    startDrag(dragStartPosition);
}

void VideoTable::startDrag(const QPoint &pos)
{
    QModelIndex index = tableWidget->indexAt(pos);
    if (!index.isValid())
        return;

    QByteArray itemData = serializeRow(index.row());


    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}

QByteArray VideoTable::serializeRow(int row) const
{
    QTableWidgetItem *pathItem = tableWidget->item(row, 3);
    QTableWidgetItem *durationItem = tableWidget->item(row, 1);

    QString path = pathItem ? pathItem->data(Qt::UserRole).toString() : "";

    QByteArray itemData;
    QDataStream stream(&itemData, QIODevice::WriteOnly);
    stream << row << pathItem->text() << durationItem->text() << pathItem->data(Qt::UserRole).toString();

    return itemData;
}