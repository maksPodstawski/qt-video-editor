//
// Created by czest on 07.02.2025.
//

#include "../include/AddTextDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
AddTextDialog::AddTextDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    textEdit = new QLineEdit(this);
    mainLayout->addWidget(new QLabel("Treść:", this));
    mainLayout->addWidget(textEdit);

    xPosSpinBox = new QSpinBox(this);
    yPosSpinBox = new QSpinBox(this);
    xPosSpinBox->setMaximum(10000);
    yPosSpinBox->setMaximum(10000);
    mainLayout->addWidget(new QLabel("Pozycja X:", this));
    mainLayout->addWidget(xPosSpinBox);
    mainLayout->addWidget(new QLabel("Pozycja Y:", this));
    mainLayout->addWidget(yPosSpinBox);

    fontSizeSpinBox = new QSpinBox(this);
    fontSizeSpinBox->setMaximum(1000);
    mainLayout->addWidget(new QLabel("Rozmiar czcionki:", this));
    mainLayout->addWidget(fontSizeSpinBox);

    fontComboBox = new QFontComboBox(this);
    mainLayout->addWidget(new QLabel("Rodzaj czcionki:", this));
    mainLayout->addWidget(fontComboBox);

    startTimeSpinBox = new QDoubleSpinBox(this);
    startTimeSpinBox->setMaximum(10000.0);
    startTimeSpinBox->setDecimals(3);
    mainLayout->addWidget(new QLabel("Czas rozpoczęcia (s):", this));
    mainLayout->addWidget(startTimeSpinBox);

    endTimeSpinBox = new QDoubleSpinBox(this);
    endTimeSpinBox->setMaximum(10000.0);
    endTimeSpinBox->setDecimals(3);
    mainLayout->addWidget(new QLabel("Czas zakończenia (s):", this));
    mainLayout->addWidget(endTimeSpinBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Anuluj", this);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString AddTextDialog::getText() const {
    return textEdit->text();
}

QPoint AddTextDialog::getPosition() const {
    return QPoint(xPosSpinBox->value(), yPosSpinBox->value());
}

int AddTextDialog::getFontSize() const {
    return fontSizeSpinBox->value();
}

QFont AddTextDialog::getFont() const {
    return fontComboBox->currentFont();
}

double AddTextDialog::getStartTime() const {
    return startTimeSpinBox->value();
}

double AddTextDialog::getEndTime() const {
    return endTimeSpinBox->value();
}