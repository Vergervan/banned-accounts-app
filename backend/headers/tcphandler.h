#ifndef TCP_HANDLER
#define TCP_HANDLER

#include <iostream>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include "idatahandler.h"

class TcpHandler : public QObject
{
	Q_OBJECT;

	public slots:
		void read();
	signals:
        void finishedRead(qintptr socketDscr, QString msg);
	private:
		inline ~TcpHandler() { delete _socket; _socket = nullptr; }
		QTcpSocket* _socket = nullptr;
	public:
		TcpHandler(qintptr socket);
		inline QTcpSocket& socket() { return *_socket; }
};
#endif
