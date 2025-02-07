#include "../include/VideoTable.h"

VideoTable::VideoTable(QWidget *parent)
    : QWidget{parent},
    tableView(new QTableView(this))
{
    setUpTableWidget();
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    setLayout(layout);

    connect(tableView, &QTableView::customContextMenuRequested, this, &VideoTable::showContextMenu);
    setupShortcuts();
}

void VideoTable::setUpTableWidget()
{
    tableModel = new QStandardItemModel(this);
    tableModel->setColumnCount(4);
    tableModel->setHorizontalHeaderLabels({"Title", "Duration", "Format", "Path"});

    tableView->setModel(tableModel);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setDragEnabled(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setDragDropMode(QAbstractItemView::DragOnly);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableView->hideColumn(3);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void VideoTable::updateTable(const QList<QString> &mediaFiles)
{
    int currentRowCount = tableModel->rowCount();
    tableModel->setRowCount(currentRowCount + mediaFiles.size());

    for(int i = 0; i < mediaFiles.size(); ++i)
    {
        const QString &filePath = mediaFiles[i];
        QFileInfo fileInfo(filePath);

        QString title = fileInfo.baseName();
        QString durationText = getDurationText(mediaFiles[i]);
        QString format = getFileFormat(mediaFiles[i]);

        QStandardItem *titleItem = new QStandardItem(title);
        QStandardItem *durationItem = new QStandardItem(durationText);
        QStandardItem *formatItem = new QStandardItem(format);
        QStandardItem *pathItem = new QStandardItem(filePath);

        titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
        durationItem->setFlags(durationItem->flags() & ~Qt::ItemIsEditable);
        formatItem->setFlags(formatItem->flags() & ~Qt::ItemIsEditable);

        titleItem->setData(filePath, Qt::UserRole);

        int newRow = currentRowCount + i;
        tableModel->setItem(newRow, 0, titleItem);
        tableModel->setItem(newRow, 1, durationItem);
        tableModel->setItem(newRow, 2, formatItem);
        tableModel->setItem(newRow, 3, pathItem);
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

    QModelIndex index = tableView->indexAt(dragStartPosition);
    if (!index.isValid())
        return;

    startDrag(dragStartPosition);
}

void VideoTable::startDrag(const QPoint &pos)
{
    QModelIndex index = tableView->indexAt(pos);
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
    QStandardItem  *titleItem = tableModel->item(row, 0);
    QStandardItem  *durationItem = tableModel->item(row, 1);
    QStandardItem  *formatItem = tableModel->item(row, 2);
    QStandardItem *pathItem = tableModel->item(row, 3);

    QString duration = durationItem ? durationItem->text() : "";
    QString title = titleItem ? titleItem->text() : "";
    QString format = formatItem ? formatItem->text() : "";
    QString path = pathItem ? pathItem->data(Qt::UserRole).toString() : "";

    QByteArray itemData;
    QDataStream stream(&itemData, QIODevice::WriteOnly);
    stream << row << titleItem->text() << durationItem->text() << formatItem->text() << pathItem->data(Qt::UserRole).toString();

    return itemData;
}


void VideoTable::showContextMenu(const QPoint& pos)
{
    QModelIndex index = tableView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    QMenu contextMenu(this);
    QAction *showFileInfoAction = contextMenu.addAction("Show File Info");
    showFileInfoAction->setShortcut(QKeySequence("Alt+Enter"));
    showFileInfoAction->setShortcutVisibleInContextMenu(true);


    QAction *removeVideo = contextMenu.addAction("Remove Video");
    QAction *selectedAction = contextMenu.exec(tableView->viewport()->mapToGlobal(pos));

    if (selectedAction == showFileInfoAction) {
        showFileInfo();
    }
    else if (selectedAction == removeVideo) {
        deleteSelectedVideo();
    }
}

void VideoTable::setupShortcuts()
{
    QShortcut *showFileInfoShortcut = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_Return), this);
    connect(showFileInfoShortcut, &QShortcut::activated, this, &VideoTable::showFileInfo);

}

void VideoTable::playSelectedVideo()
{
    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();
    for (const QModelIndex &index : selectedRows) {
        QString filePath = tableModel->item(index.row(), 0)->data(Qt::UserRole).toString();
        emit playVideoRequested(filePath);
    }
}


void VideoTable::deleteSelectedVideo() {
    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();
    for (const QModelIndex &index : selectedRows) {
        QString filePath = tableModel->item(index.row(), 0)->data(Qt::UserRole).toString();
        emit videoRemoved(filePath);
        tableModel->removeRow(index.row());
    }
}

void VideoTable::showFileInfo()
{
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        return;
    }

    QModelIndex index = selectedIndexes.first();
    QString filePath = tableModel->item(index.row(), 0)->data(Qt::UserRole).toString();
    QFileInfo fileInfo(filePath);
    QString title = fileInfo.baseName();
    QString durationText = getDurationText(filePath);
    QString format = fileInfo.suffix().toUpper();
    qint64 fileSize = fileInfo.size();
    QString fileSizeText;

    if (fileSize < 1024 * 1024) {
        fileSizeText = QString::number(fileSize / 1024.0, 'f', 2) + " KB";
    } else if (fileSize < 1024 * 1024 * 1024) {
        fileSizeText = QString::number(fileSize / (1024.0 * 1024.0), 'f', 2) + " MB";
    } else {
        fileSizeText = QString::number(fileSize / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
    }

    QString message = QString("Title: %1\nDuration: %2\nFormat: %3\nPath: %4\nSize: %5")
                     .arg(title)
                     .arg(durationText)
                     .arg(format)
                     .arg(filePath)
                     .arg(fileSizeText);

    QMessageBox::information(this, "File Info", message);
}
