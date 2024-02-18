QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chipplanner.cpp \
    editor.cpp \
    infowidget.cpp \
    main.cpp \
    mainwindow.cpp \
    projectnavigator.cpp \
    projectwizard.cpp \
    taskview.cpp

HEADERS += \
    chipplanner.h \
    editor.h \
    infowidget.h \
    mainwindow.h \
    projectnavigator.h \
    projectwizard.h \
    taskview.h

HEADERS += ./Qsci/qsciscintilla.h \
    ./Qsci/qsciscintillabase.h \
    ./Qsci/qscilexercpp.h \
    ./Qsci/qscilexercustom.h \
    ./Qsci/qscilexer.h \
    ./Qsci/qsciabstractapis.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    chipplanner.ui \
    infowidget.ui \
    mainwindow.ui \
    projectnavigator.ui \
    projectwizard.ui \
    taskview.ui

INCLUDEPATH += $$PWD/Qsci

LIBS += -L$$PWD/libs -lqscintilla2_qt5

RESOURCES += \
    res.qrc
