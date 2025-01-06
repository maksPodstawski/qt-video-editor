//
// Created by czest on 06.01.2025.
//

#include "../include/Indicator.h"
#include <QPainter>
#include <QMouseEvent>

Indicator::Indicator(QWidget *parent)
        : QWidget(parent), dragging(false)
{
    setFixedSize(10, 20);
}

void Indicator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::blue);
    painter.drawRect(rect());
}

void Indicator::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragging = true;
        dragStartPos = event->pos();
    }
}

void Indicator::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging)
    {
        int newX = x() + event->pos().x() - dragStartPos.x();
        newX = qMax(0, qMin(newX, parentWidget()->width() - width()));
        move(newX, y());
        qDebug() << "Indicator moved to: " << getPostion();
        emit parentWidget()->update();
    }
}

void Indicator::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragging = false;
    }
}

QPoint Indicator::getPostion() const {
    return pos();
}
