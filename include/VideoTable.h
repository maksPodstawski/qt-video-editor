#ifndef VIDEOTABLEWIDGET_H
#define VIDEOTABLEWIDGET_H

#include "VideoData.h"
#include "VideoTable.h"
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QTableWidgetItem>
#include <QMediaPlayer>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QMimeData>
#include <QDrag>


class VideoTable : public QWidget
{
    Q_OBJECT
public:
    explicit VideoTable(QWidget *parent = nullptr);


    void updateTable(const QList<QString> &mediaFiles);

    static QString getDurationText(const QString &filePath);
    static QString getFileFormat(const QString &filePath);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QTableWidget *tableWidget;
    QPoint dragStartPosition;
    QByteArray serializeRow(int row) const;

    void startDrag(const QPoint &pos);
    void setUpTableWidget();
};

#endif // VIDEOTABLEWIDGET_H
