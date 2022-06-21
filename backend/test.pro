CONFIG += qt debug console

MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
UI_DIR      = build/ui
RCC_DIR     = build/

HEADERS += \
			headers/apihandler.h \
			headers/idatahandler.h \
			headers/tcphandler.h \
			headers/tcplistener.h


SOURCES += \
			src/main.cpp \
			src/apihandler.cpp \
			src/tcphandler.cpp \
			src/tcplistener.cpp


QT += core network sql
