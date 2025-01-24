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
        qDebug() << "Indicator moved to: " << getPosition();
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

QPoint Indicator::getPosition() const {
    return pos();
}

void Indicator::contextMenuEvent(QContextMenuEvent *event) {
    QMenu contextMenu(this);
    QAction *getVideoAction = contextMenu.addAction("Get Current Video");
    QAction *cutRightAction = contextMenu.addAction("Cut Right");
    QAction *cutLeftAction = contextMenu.addAction("Cut Left");
    QAction *splitAction =  contextMenu.addAction("Split");

    connect(cutRightAction, &QAction::triggered, this, &Indicator::cutRight);
    connect(cutLeftAction, &QAction::triggered, this, &Indicator::cutLeft);
    connect(splitAction, &QAction::triggered, this, &Indicator::split);

    connect(getVideoAction, &QAction::triggered, this, &Indicator::requestCurrentVideo);
    contextMenu.exec(event->globalPos());
}
