#include "videotablewidget.h"

VideoTableWidget::VideoTableWidget(QWidget *parent)
    : QWidget{parent},
    tableWidget(new QTableWidget(this))
{
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Tytuł" << "Czas trwania");

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

        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(Qt::UserRole, videoFiles[i]);
        tableWidget->setItem(i, 0, titleItem);

        QTableWidgetItem *durationItem = new QTableWidgetItem(durationText);
        tableWidget->setItem(i, 1, durationItem);
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

void VideoTableWidget::startDrag(Qt::DropActions supportedActions)
{
    int currentRow = tableWidget->currentRow();

    QTableWidgetItem *nameItem = tableWidget->item(currentRow, 0);
    if (!nameItem)
        return;

    QTableWidgetItem *durationItem = tableWidget->item(currentRow, 1);
    QString duration;
    if (durationItem) {
        duration = durationItem->text();
        qDebug() << "Duration found:" << duration;
    } else {
        qDebug() << "No duration item in column 1";
        return;
    }

    QString filePath = nameItem->data(Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        qDebug() << "No file path stored in item";
        return;
    }

    qDebug() << "Preparing drag data:";
    qDebug() << "Name:" << nameItem->text();
    qDebug() << "Duration:" << duration;
    qDebug() << "FilePath:" << filePath;

    QByteArray itemData;
    QDataStream stream(&itemData, QIODevice::WriteOnly);
    stream << currentRow << nameItem->text() << duration << filePath;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(supportedActions);
}
