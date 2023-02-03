QT += quick \
      sql \
      quickcontrols2 \
      core

CONFIG += \
            c++11 \
            qml_debug

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    authorizationmanager.h \
    configurator.h \
    idatacryptor.h

SOURCES += \
        authorizationmanager.cpp \
        configurator.cpp \
        main.cpp

RESOURCES += qml.qrc

TARGET = BannedAccounts

win32:LIBS += -lAdvapi32

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

RC_ICONS = appicon.ico
win32:VERSION = 1.0.0.0
QMAKE_TARGET_DESCRIPTION = Banned Accounts
QMAKE_TARGET_COMPANY = Vergervan
# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES +=
