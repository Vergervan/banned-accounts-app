#include "../headers/tcplistener.h"

TcpListener::TcpListener()
{
	// int dscr = socket(AF_INET, SOCK_STREAM, 0);
	// int val = 1;
	// setsockopt(dscr, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	// setsockopt(dscr, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
	// this->setSocketDescriptor(dscr);
}

void TcpListener::messageProc(qintptr socketDscr, QString msg)
{
    this->_handler->handleData(socketDscr, msg);
}

void TcpListener::incomingConnection(qintptr socketDscrpt)
{
	std::cout << "New connection " << socketDscrpt << std::endl;
	TcpHandler* handler = new TcpHandler(socketDscrpt);	
	QThread* thread = new QThread;
	handler->moveToThread(thread);
	connect(handler, &TcpHandler::finishedRead, this, &TcpListener::messageProc);
	connect(thread, &QThread::finished, handler, &TcpHandler::deleteLater);

	thread->start();
}
