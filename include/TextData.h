//
// Created by czest on 07.02.2025.
//

#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QFont>
#include <QRect>
#include <QString>

class TextData {
public:
    TextData(const QString &text, const QFont &font, int fontSize, const QRect &rect, int posX, int posY);

    QString getText() const;
    QFont getFont() const;
    int getFontSize() const;
    QRect getRect() const;
    int getPosX() const;
    int getPosY() const;

    void setText(const QString &text);
    void setFont(const QFont &font);
    void setFontSize(int fontSize);
    void setRect(const QRect &rect);

    qreal getStartTime() const;
    qreal getEndTime() const;

private:
    QString text;
    QFont font;
    int fontSize;
    QRect rect;
    qreal startTime;
    qreal endTime;
    int posX;
    int posY;
};

#endif //TEXTDATA_H
