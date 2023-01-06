#include <iostream>
#include <map>
#include <fstream>
#include <QObject>
#include <QSqlDatabase>
#include <QtSql>
#include <qcoreapplication.h>
#include "../headers/appnetworkhandler.h"
#include "../headers/tcplistener.h"
#include "../headers/config.h"

void initDb(QSqlDatabase& db)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("banned_accounts.db");
    if(!db.open()){
        std::cout << db.lastError().text().toStdString() << std::endl;
        return;
    }
    //db.exec("PRAGMA locking_mode = EXCLUSIVE");
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS users(username TEXT PRIMARY KEY, pass_hash TEXT NOT NULL, time_stamp TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP)");
    query.exec("CREATE TABLE IF NOT EXISTS accounts(nick TEXT NOT NULL, login TEXT NOT NULL, unban_time TEXT, owner_user TEXT, time_stamp TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY(nick, login), FOREIGN KEY(owner_user) REFERENCES users(username))");
//    query.exec("CREATE TRIGGER IF NOT EXISTS update_account_time BEFORE UPDATE ON accounts \
//               BEGIN \
//                  UPDATE accounts SET new.unban_time = datetime(new.unban_time, 'localtime'), time_stamp = datetime('now', 'localtime') \
//                  WHERE rowid = new.rowid; \
//               END");
    query.exec("CREATE TRIGGER IF NOT EXISTS insert_account_time AFTER INSERT ON accounts \
               BEGIN \
                  UPDATE accounts SET unban_time = datetime(new.unban_time, 'localtime'), time_stamp = datetime('now', 'localtime') \
                  WHERE rowid = new.rowid; \
               END");
}

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
    qRegisterMetaType<IDataHandler::Message>("IDataHandler::Message");
    QSqlDatabase db;
    initDb(db);

	std::ifstream readfile("net.conf");
	if(!readfile.good() || !readfile.is_open())
	{
		std::cout << "Cannot read a configuration file" << std::endl;
		std::cout << "It will be created automatically under the name 'net.conf' with the basic settings" << std::endl;
#ifdef Q_OS_LINUX
		system("touch net.conf & echo 'host=0.0.0.0\\nport=25000' > net.conf");
#endif
#ifdef Q_OS_WINDOWS
        system("type nul > net.conf");
        system("echo host=0.0.0.0 >> net.conf");
        system("echo port=25000 >> net.conf");
#endif
        return 0;
	}
	Config cfg = readcfg("net.conf");
	std::cout << "Config: " << cfg.address.toString().toStdString() << ':' << cfg.port << std::endl;

	TcpListener* listener = new TcpListener;
    AppNetworkHandler* handler = new AppNetworkHandler(&db);

	QObject::connect(listener, &TcpListener::acceptError, [=](const QAbstractSocket::SocketError& err){ std::cout << "Accept error: " << err << std::endl;});
    QObject::connect(handler, &IDataHandler::sendPong, listener, &TcpListener::pongHandle);
    QObject::connect(handler, &AppNetworkHandler::sendRemoveClient, listener, &TcpListener::removeClient);

	listener->setDataHandler(handler);
	listener->listen(cfg.address, cfg.port);

	if(!listener->isListening())
	{
		std::cout << "Server cannot start listening " << listener->errorString().toStdString() << std::endl;
		return 0;
	}
	std::cout << "Listening on: " << listener->serverAddress().toString().toStdString() << ':' << listener->serverPort() << std::endl;

    const int resultCode = app.exec();
    std::cout << "App code: " << resultCode << std::endl;
    db.close();
    handler->deleteLater();
    listener->deleteLater();
	// delete listener;
	// delete handler;
    return resultCode;
}
