#include "../headers/tcplistener.h"
#include "../headers/idatahandler.h"
#include <string>

#define PING_TIME 5000

TcpListener::TcpListener()
{
	// int dscr = socket(AF_INET, SOCK_STREAM, 0);
	// int val = 1;
	// setsockopt(dscr, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	// setsockopt(dscr, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
    // this->setSocketDescriptor(dscr);
    _pinger.callOnTimeout([&, this](){
        if(_pongerSet.count() != 0){
            for(auto con = _pongerSet.begin(); con != _pongerSet.end(); con++) {
                std::cout << "Socket " << con.value() << " disconnected" << std::endl;
                _currentConnections.remove(con.value());
                con.key()->close();
                con.key()->deleteLater();
                //system("Color 0A");
                std::cout << "Current connections: " << _currentConnections.count() << std::endl;
            }
            _pongerSet.clear();
        }
        if(this->_currentConnections.count() == 0) return;
        for(auto con = _currentConnections.begin(); con != _currentConnections.end(); con++){
            QDataStream stream(con.value());
            stream << (ushort)10;
            _pongerSet.insert(con.value(), con.key());
    //            con.value()->write("0");
    //            if(!con.value()->flush()){
    //                std::cout << "Socket " << con.key() << " disconnected" << std::endl;
    //                con.value()->close();
    //                con.value()->deleteLater();
    //                _currentConnections.remove(con.key());
    //                break;
    //            }
        }
    });
    _pinger.start(PING_TIME);
}

void TcpListener::pongHandle(QTcpSocket* socket){
    if(_pongerSet.contains(socket)){
        _pongerSet.remove(socket);
    }
}

void TcpListener::removeClient(qintptr dscr){
    QTcpSocket* socket = _currentConnections[dscr];
    if(_pongerSet.contains(socket))
        _pongerSet.remove(socket);
    std::cout << "Socket " << dscr << " disconnected" << std::endl;
    _currentConnections.remove(dscr);
}

void TcpListener::messageProc(QTcpSocket* socket, IDataHandler::Message msg)
{
    //std::cout << socketDscr << " " << msg.toStdString() << std::endl;
    this->_handler->handleData(socket, msg);
}

void TcpListener::incomingConnection(qintptr socketDscrpt)
{
    QTcpSocket* socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDscrpt);
    _currentConnections.insert(socketDscrpt, socket);
//    connect(socket, &QTcpSocket::errorOccurred, socket, [&](QAbstractSocket::SocketError err){
//        std::cout << socket->errorString().toStdString() << std::endl;
//        socket->deleteLater();
//    });
	std::cout << "New connection " << socketDscrpt << std::endl;
    TcpHandler* handler = new TcpHandler(socket);
	QThread* thread = new QThread;
	handler->moveToThread(thread);
	connect(handler, &TcpHandler::finishedRead, this, &TcpListener::messageProc);

	thread->start();
}
