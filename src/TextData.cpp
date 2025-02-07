//
// Created by czest on 07.02.2025.
//

#include "../include/TextData.h"


TextData::TextData(const QString &text, const QFont &font, int fontSize, const QRect &rect, int posX, int posY)
    : text(text), font(font), fontSize(fontSize), rect(rect), posX(posX), posY(posY) {
}

QString TextData::getText() const {
    return text;
}

QFont TextData::getFont() const {
    return font;
}

int TextData::getFontSize() const {
    return fontSize;
}

QRect TextData::getRect() const {
    return rect;
}

int TextData::getPosX() const {
    return this->posX;
}

int TextData::getPosY() const {
    return this->posY;
}

void TextData::setText(const QString &text) {
    this->text = text;
}

void TextData::setFont(const QFont &font) {
    this->font = font;
}

void TextData::setFontSize(int fontSize) {
    this->fontSize = fontSize;
}

void TextData::setRect(const QRect &rect) {
    this->rect=rect;
}

qreal TextData::getStartTime() const {
    return startTime;
}

qreal TextData::getEndTime() const {
    return endTime;
}
