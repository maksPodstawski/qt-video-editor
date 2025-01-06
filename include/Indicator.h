//
// Created by czest on 06.01.2025.
//

#ifndef VIDEO_EDITOR_INDICATOR_H
#define VIDEO_EDITOR_INDICATOR_H


#include <QWidget>

class Indicator : public QWidget
{
Q_OBJECT
public:
    explicit Indicator(QWidget *parent = nullptr);
    QPoint getPostion() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool dragging;
    QPoint dragStartPos;
};


#endif //VIDEO_EDITOR_INDICATOR_H
