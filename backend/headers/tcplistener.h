#ifndef TCP_LISTENER
#define TCP_LISTENER

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QSet>
#include <QMap>
#include <QTimer>
#include <QDataStream>
#include <iostream>
#include "tcphandler.h"
#include "idatahandler.h"
#include "imessagequeue.h"
#include <queue>
#include <QSet>

#ifdef Q_OS_LINUX
#include <sys/socket.h>
#endif
#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

class TcpListener : public QTcpServer//, public IMessageQueue
{
	public:
        struct MessageToSend
        {
            QTcpSocket* socket;
            IDataHandler::Message msg;
        };
        struct Ponger{
            qintptr dscr;
            QTcpSocket* socket;
        };

        TcpListener();
		inline void setDataHandler(IDataHandler* handler) { _handler = handler; }
		inline ~TcpListener(){}
        //void enqueueMessage(QTcpSocket*, IDataHandler::Message) override;
	protected:
		void incomingConnection(qintptr socket) Q_DECL_OVERRIDE;
	public slots:
        void messageProc(QTcpSocket* socket, IDataHandler::Message msg);
        void pongHandle(QTcpSocket* socket);
        void removeClient(qintptr dscr);
	private:
		IDataHandler* _handler = nullptr;
        QSet<QTcpSocket> _socketSet;
        QMap<qintptr, QTcpSocket*> _currentConnections;
        QTimer _pinger;
        QMap<QTcpSocket*, qintptr> _pongerSet;
        std::queue<MessageToSend> msgQueue;
};
#endif
