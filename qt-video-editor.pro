QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/TimeLine.cpp \
    src/VideoData.cpp \
    src/VideoPerview.cpp \
    src/VideoTable.cpp \
    src/main.cpp \
    src/mainwindow.cpp \


HEADERS += \
    include/TimeLine.h \
    include/VideoData.h \
    include/VideoPerview.h \
    include/VideoTable.h \
    include/mainwindow.h \
    ui/ui_mainwindow.h \

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    CMakeLists.txt \
    styles/app.qss

