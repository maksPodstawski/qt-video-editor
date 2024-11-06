#ifndef VIDEOTABLEWIDGET_H
#define VIDEOTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QTableWidgetItem>
#include <QMediaPlayer>
#include <QEventLoop>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

class VideoTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoTableWidget(QWidget *parent = nullptr);
    void updateTable(const QList<QString> &videoFiles);
    static QString getDurationText(const QString &filePath);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QTableWidget *tableWidget;
    void startDrag(const QPoint &pos);
    QByteArray serializeRow(int row) const;
    QPoint dragStartPosition;


};

#endif // VIDEOTABLEWIDGET_H
