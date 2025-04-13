QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L"F:\Microsoft Visual Studio\OpenSSL-Win64\lib\VC\x64\MD" # -llibcrypto # 库文件名及其路径
LIBS += -L"F:\Microsoft Visual Studio\OpenSSL-Win64\lib\VC\x64\MT" -llibcrypto -llibssl # 库文件名及其路径
#INCLUDEPATH += $$quote("F:\Microsoft Visual Studio\OpenSSL-Win64\include") #头文件路径
INCLUDEPATH += "F:\Microsoft Visual Studio\OpenSSL-Win64\include"


SOURCES += \
    common/loginfo.cpp \
    common/cryptutil.cpp \
    common/network_manager.cpp \
    common/logininfoinstance.cpp \
    login/login.cpp \
    login/logincontext.cpp \
    login/registercontext.cpp \
    login/serverconfig.cpp \
    login/titlewg.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    common/global.h \
    common/loginfo.h \
    common/cryptutil.h \
    common/network_manager.h \
    common/logininfoinstance.h \
    login/login.h \
    login/logincontext.h \
    login/registercontext.h \
    login/serverconfig.h \
    login/titlewg.h \
    mainwindow.h

# lrelease YunDisk_zh_CN.ts -qm YunDisk_zh_CN.qm
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
