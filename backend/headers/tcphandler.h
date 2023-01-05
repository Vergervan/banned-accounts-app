#ifndef TCP_HANDLER
#define TCP_HANDLER

#include <iostream>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include "idatahandler.h"

class TcpHandler : public QObject
{
    Q_OBJECT

	public slots:
		void read();
	signals:
        void finishedRead(QTcpSocket* socket, IDataHandler::Message msg);
    private:
		QTcpSocket* _socket = nullptr;
	public:
        TcpHandler(QTcpSocket* socket);
		inline QTcpSocket& socket() { return *_socket; }
};
#endif
