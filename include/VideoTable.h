#ifndef VIDEOTABLE_H
#define VIDEOTABLE_H

#include "VideoData.h"
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QShortcut>


class VideoTable : public QWidget
{
    Q_OBJECT

public:
    explicit VideoTable(QWidget* parent = nullptr);

    QList<QString> getLoadedVideos() const;
    static QString getDurationText(const QString& filePath);
    static QString getFileFormat(const QString& filePath);

    void updateTable(const QList<QString>& mediaFiles);
    void deleteSelectedVideo();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QTableView* tableView;
    QStandardItemModel* tableModel;
    QPoint dragStartPosition;

    void setupShortcuts();
    void setUpTableWidget();
    void startDrag(const QPoint& pos);
    void showContextMenu(const QPoint& pos);
    void showFileInfo();
    QByteArray serializeRow(int row) const;

signals:
    void videoRemoved(const QString& filePath);
};

#endif // VIDEOTABLE_H
