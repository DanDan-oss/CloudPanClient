QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    login/login.cpp \
    login/logincontext.cpp \
    login/registercontext.cpp \
    login/serverconfig.cpp \
    login/titlewg.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    global.h \
    login/login.h \
    login/logincontext.h \
    login/registercontext.h \
    login/serverconfig.h \
    login/titlewg.h \
    macro.h \
    mainwindow.h

TRANSLATIONS += \
    YunDisk_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    res.qrc
