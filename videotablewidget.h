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

class VideoTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoTableWidget(QWidget *parent = nullptr);
    void updateTable(const QList<QString> &videoFiles);

protected:
    void startDrag(Qt::DropActions supportedActions);

private:
    QString getDurationText(const QString &filePath);
    QTableWidget *tableWidget;


signals:
};

#endif // VIDEOTABLEWIDGET_H
