#ifndef VIDEO_EDITOR_INDICATOR_H
#define VIDEO_EDITOR_INDICATOR_H

#include <QWidget>
#include <QMenu>
#include <QPainter>
#include <QMouseEvent>

class Indicator : public QWidget
{
    Q_OBJECT

public:
    explicit Indicator(QWidget* parent = nullptr);
    QPointF getPosition() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    bool dragging;
    QPointF dragStartPos;

signals:
    void requestCurrentVideo();
    void cutRight();
    void cutLeft();
    void split();
    void indicatorReleased();
};

#endif //VIDEO_EDITOR_INDICATOR_H
