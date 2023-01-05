#include "../headers/tcphandler.h"
#include "../headers/idatahandler.h"
#include "../headers/appnetworkhandler.h"

TcpHandler::TcpHandler(QTcpSocket* socket)
{
    _socket = socket;
	connect(_socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void TcpHandler::read()
{
	QByteArray total_data, data_buffer;
    IDataHandler::Message msg;
    QDataStream stream(this->_socket);
    ushort code;
    stream >> code;
    msg.code = static_cast<AppNetworkHandler::MessageCode>(code);
    while(true)
    {
        data_buffer = _socket->read(1024);
        if(data_buffer.isEmpty()) {
            break;
        }
        total_data.append(data_buffer);
    }
    msg.data = QString(total_data);
    emit finishedRead(_socket, msg);
}
