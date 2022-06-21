#ifndef TCP_LISTENER
#define TCP_LISTENER

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <sys/socket.h>
#include <iostream>
#include "tcphandler.h"
#include "idatahandler.h"

class TcpListener : public QTcpServer
{
	public:
		TcpListener();
		inline void setDataHandler(IDataHandler* handler) { _handler = handler; }
		inline ~TcpListener(){}
	protected:
		void incomingConnection(qintptr socket) Q_DECL_OVERRIDE;
	public slots:
		void messageProc (QString msg);
	private:
		IDataHandler* _handler = nullptr;
};
#endif
