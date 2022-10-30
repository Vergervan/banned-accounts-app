#include "../headers/tcphandler.h"

TcpHandler::TcpHandler(qintptr dscr)
{
	_socket = new QTcpSocket;	
	_socket->setSocketDescriptor(dscr);
	connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void TcpHandler::read()
{
	std::cout << "Read " << _socket->socketDescriptor() << std::endl;
	QByteArray total_data, data_buffer;
    while(true)
    {
        data_buffer = _socket->read(1024);
        if(data_buffer.isEmpty()) {
            break;
        }
        total_data.append(data_buffer);
    }
    emit finishedRead(_socket->socketDescriptor(), QString(total_data));
}
