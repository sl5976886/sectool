#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T10:45:13
#
#-------------------------------------------------

QT       += core gui sql xml axcontainer network

LIBS += -l WS2_32

RC_FILE = uac.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SecToolUI
TEMPLATE = app


SOURCES += main.cpp\
    registerdialog.cpp \
    logindialog.cpp \
    forgetdialog.cpp \
    maindialog.cpp \
    taskmanagedialog.cpp \
    newtaskdialog.cpp \
    modifydialog.cpp \
    modifypersondialog.cpp \
    disposedialog.cpp \
    editrecorddialog.cpp \
    sysmanagedialog.cpp \
    webtablewidgeteditdialog.cpp \
    newuserdialog.cpp \
    newcomdialog.cpp \
    commonfunction.cpp \
    editwebdialog.cpp \
    editsecdialog.cpp \
    editsafedialog.cpp \
    toolsetdialog.cpp \
    addhostdialog.cpp \
    cleanlogdialog.cpp \
    renderthread.cpp \
    worddialog.cpp \
    starttool.cpp \
    disposedialog2.cpp \
    buttondelegate.cpp \
    tablemodel.cpp \
    cmyipaddredit.cpp \
    cmyippartlineedit.cpp \
    waitdialog.cpp \
    reportthread.cpp \
    adddispersondialog.cpp \
    ccrashstack.cpp

HEADERS  += \
    registerdialog.h \
    logindialog.h \
    forgetdialog.h \
    maindialog.h \
    taskmanagedialog.h \
    newtaskdialog.h \
    modifydialog.h \
    modifypersondialog.h \
    disposedialog.h \
    editrecorddialog.h \
    sysmanagedialog.h \
    webtablewidgeteditdialog.h \
    connect.h \
    newuserdialog.h \
    newcomdialog.h \
    commonfunction.h \
    editwebdialog.h \
    editsecdialog.h \
    editsafedialog.h \
    toolsetdialog.h \
    addhostdialog.h \
    cleanlogdialog.h \
    renderthread.h \
    worddialog.h \
    starttool.h \
    disposedialog2.h \
    buttondelegate.h \
    tablemodel.h \
    cmyipaddredit.h \
    cmyippartlineedit.h \
    waitdialog.h \
    reportthread.h \
    adddispersondialog.h \
    ccrashstack.h

FORMS    += \
    registerdialog.ui \
    logindialog.ui \
    forgetdialog.ui \
    maindialog.ui \
    taskmanagedialog.ui \
    newtaskdialog.ui \
    modifydialog.ui \
    modifypersondialog.ui \
    disposedialog.ui \
    editrecorddialog.ui \
    sysmanagedialog.ui \
    webtablewidgeteditdialog.ui \
    newuserdialog.ui \
    newcomdialog.ui \
    editwebdialog.ui \
    editsecdialog.ui \
    editsafedialog.ui \
    toolsetdialog.ui \
    addhostdialog.ui \
    cleanlogdialog.ui \
    worddialog.ui \
    disposedialog2.ui \
    waitdialog.ui \
    adddispersondialog.ui

RESOURCES += \
    image.qrc \
    style.qrc \
    qss.qrc \
    button.qrc

win32:RC_FILE = myico1.rc
