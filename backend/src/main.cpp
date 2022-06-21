#include <iostream>
#include <map>
#include <fstream>
#include <QObject>
#include <qcoreapplication.h>
#include "../headers/apihandler.h"
#include "../headers/tcplistener.h"
#include "../headers/config.h"

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	
	std::ifstream readfile("net.conf");
	if(!readfile.good() || !readfile.is_open())
	{
		std::cout << "Cannot read a configuration file" << std::endl;
		std::cout << "It will be created automatically under the name 'net.conf' with the basic settings" << std::endl;
		system("touch net.conf & echo 'host=0.0.0.0\\nport=25000' > net.conf");
		return 0;
	}
	Config cfg = readcfg("net.conf");
	std::cout << "Config: " << cfg.address.toString().toStdString() << ':' << cfg.port << std::endl;

	TcpListener* listener = new TcpListener;
	QObject::connect(listener, &TcpListener::acceptError, [=](const QAbstractSocket::SocketError& err){ std::cout << "Accept error: " << err << std::endl;});

	ApiHandler* handler = new ApiHandler;
	listener->setDataHandler(handler);
	listener->listen(cfg.address, cfg.port);

	if(!listener->isListening())
	{
		std::cout << "Server cannot start listening " << listener->errorString().toStdString() << std::endl;
		return 0;
	}
	std::cout << "Listening on: " << listener->serverAddress().toString().toStdString() << ':' << listener->serverPort() << std::endl;

	// delete listener;
	// delete handler;
	return app.exec();
}
