#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include "videotablewidget.h"
#include "VideoData.h"

VideoTableWidget::VideoTableWidget(QWidget *parent)
    : QWidget{parent},
    tableWidget(new QTableWidget(this))
{
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Title" << "Duration" << "Format" << "Path");
    tableWidget->setDragEnabled(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setDragDropMode(QAbstractItemView::DragOnly);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
}

void VideoTableWidget::updateTable(const QList<QString> &mediaFiles)
{
    tableWidget->setRowCount(mediaFiles.size());

    for(int i = 0; i < mediaFiles.size(); ++i) {
        QFileInfo fileInfo(mediaFiles[i]);
        QString title = fileInfo.fileName();
        QString durationText = getDurationText(mediaFiles[i]);
        QString format = getFileFormat(mediaFiles[i]);
        QString filePath = mediaFiles[i];

        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(Qt::UserRole, filePath);
        tableWidget->setItem(i, 0, titleItem);

        QTableWidgetItem *durationItem = new QTableWidgetItem(durationText);
        tableWidget->setItem(i, 1, durationItem);

        QTableWidgetItem *formatItem = new QTableWidgetItem(format);
        tableWidget->setItem(i, 2, formatItem);

        QTableWidgetItem *pathItem = new QTableWidgetItem(filePath);
        tableWidget->setItem(i, 3, pathItem);
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

QString VideoTableWidget::getFileFormat(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix().toUpper();
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

QByteArray VideoTableWidget::serializeRow(int row) const
{
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


