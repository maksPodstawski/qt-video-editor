//
// Created by czest on 07.02.2025.
//

#ifndef ADDTEXTDIALOG_H
#define ADDTEXTDIALOG_H

#include <QDialog>
#include <QFont>
#include <QLineEdit>
#include <QSpinBox>
#include <QFontComboBox>
#include <QPushButton>

class AddTextDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTextDialog(QWidget *parent = nullptr);

    QString getText() const;
    QPoint getPosition() const;
    int getFontSize() const;
    QFont getFont() const;
    double getStartTime() const;
    double getEndTime() const;

private:
    QLineEdit *textEdit;
    QSpinBox *xPosSpinBox;
    QSpinBox *yPosSpinBox;
    QSpinBox *fontSizeSpinBox;
    QFontComboBox *fontComboBox;
    QDoubleSpinBox *startTimeSpinBox;
    QDoubleSpinBox *endTimeSpinBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};


#endif //ADDTEXTDIALOG_H
