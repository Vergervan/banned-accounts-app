CONFIG += qt debug console

MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
UI_DIR      = build/ui
RCC_DIR     = build/

HEADERS += \
			headers/apihandler.h \
			headers/appnetworkhandler.h \
			headers/idatacryptor.h \
			headers/idatahandler.h \
			headers/imessagequeue.h \
			headers/qsimplecrypt.h \
			headers/simplecryptor.h \
			headers/tcphandler.h \
                        headers/tcplistener.h \
                        headers/config.h


SOURCES += \
			src/appnetworkhandler.cpp \
			src/main.cpp \
			src/apihandler.cpp \
			src/qsimplecrypt.cpp \
			src/simplecryptor.cpp \
			src/tcphandler.cpp \
			src/tcplistener.cpp


QT += core network sql

win32{
    LIBS += -lws2_32
}
