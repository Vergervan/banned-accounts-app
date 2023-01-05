#include "../headers/appnetworkhandler.h"

AppNetworkHandler::AppNetworkHandler()
{
    _cryptor = new SimpleCryptor(0x04534FAB057D);
}

AppNetworkHandler::~AppNetworkHandler()
{
    delete _cryptor;
}

void AppNetworkHandler::handleData(QTcpSocket* socket, IDataHandler::Message msg)
{
    if(msg.code != Pong)
        std::cout << "Message Code: " << msg.code << std::endl;
    QDataStream stream(socket);
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(msg.data.toUtf8());
    obj = doc.object();
    Message resultMsg;
    switch(msg.code)
    {
       case SignIn:
       case SignUp:
          resultMsg = signHandle(msg.code, obj["username"].toString(), obj["pass"].toString());
          stream << getRawCode(resultMsg.code);
          socket->write(resultMsg.data.toStdString().c_str());
          break;
       case AddAccount:
          std::cout << "Trying to add account" << std::endl;
          if(addAccount(obj)){
            stream << getRawCode(AddAccount);
            socket->write("Account added");
          }
          break;
       case ChangeAccount:
          std::cout << "Changing account" << std::endl;
          if(updateAccount(obj)){
              std::cout << obj["nick"].toString().toStdString() << " account data updated" << std::endl;
          }
          break;
       case Pong:
          emit sendPong(socket);
       break;
    }
}

IDataHandler::Message AppNetworkHandler::signHandle(MessageCode code, QString nick, QString pass)
{
    Message msg;
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ?");
    query.addBindValue(nick);
    //query.addBindValue(pass);
    if(!query.exec() || !query.first()){
        if(code == SignUp){
            QRegularExpression regex;
            regex.setPattern("^\\w+[A-Za-z0-9_]$");
            if(!regex.match(nick).hasMatch()){
                msg.code = ErrorSignUp;
                msg.data = "Nickname has invalid characters";
                return msg;
            }
            query.clear();
            query.prepare("INSERT INTO users(username, pass_hash) VALUES(?, ?)");
            query.addBindValue(nick);
            query.addBindValue(pass);
            if(!query.exec()){
                msg.code = ErrorSignUp;
                msg.data = query.lastError().text();
            }else{
                msg.code = AcceptSignUp;
            }
        }else if(code == SignIn){
            msg.code = ErrorSignIn;
            msg.data = "User doesn't exist";
        }
        return msg;
    }
    if(code == SignUp){
        msg.code = ErrorSignUp;
        msg.data = "Nickname is already busy";
        return msg;
    }
    query.first();
    if(query.value(1).toString() == pass){
        msg.code = AcceptSignIn;
    }else{
        msg.code = ErrorSignIn;
        msg.data = "Invalid password";
    }
    return msg;
}

bool AppNetworkHandler::addAccount(const QJsonObject& obj)
{
    QString rawValue = obj["days"].toString();
    QString tempStr = QString("datetime('now', '+%1 day', '+%2 hour', '+%3 minute'").arg(rawValue.mid(0, 2), rawValue.mid(3, 2), rawValue.mid(6, 2));
    QSqlQuery query;
    query.prepare(QString("INSERT INTO accounts(nick, login, owner_user, unban_time) VALUES(?, ?, ?, %1))").arg(tempStr));
    query.addBindValue(obj["nick"].toString());
    query.addBindValue(obj["login"].toString());
    query.addBindValue(obj["username"].toString());
    if(!query.exec()){
        std::cout << query.lastError().text().toStdString() << std::endl;
        return false;
    }
    return true;
}
bool AppNetworkHandler::updateAccount(const QJsonObject& obj)
{
    QSqlQuery query;
    query.prepare("SELECT ROWID, * FROM accounts WHERE nick = ? AND login = ? AND owner_user = ?");
    query.addBindValue(obj["nick"].toString());
    query.addBindValue(obj["login"].toString());
    query.addBindValue(obj["username"].toString());
    if(!query.exec() || !query.first()){
        return false;
    }
    QVariant rowid = query.value(0);
    QString rawValue = obj["days"].toString();
    QString tempStr = QString("datetime('now', '+%1 day', '+%2 hour', '+%3 minute'").arg(rawValue.mid(0, 2), rawValue.mid(3, 2), rawValue.mid(6, 2));
    query.clear();
    query.prepare(QString("UPDATE accounts SET unban_time = %1 time_stamp = CURRENT_TIMESTAMP WHERE ROWID = ?").arg(tempStr));
    query.addBindValue(rowid);
    return query.exec();
}


//select *, (date(time_stamp, '+' || substr(unban_time, 1, 2) || ' day')) as unban_date, time(time_stamp, '+' || substr(unban_time, 4, 2) || ' hour') as time from accounts


//    qDebug() << obj["message"].toString();
//    QTcpSocket clientSocket;
//    clientSocket.setSocketDescriptor(socketDscr);
//    QJsonDocument doc;
//    doc.fromJson(QByteArray::fromStdString(msg.toStdString()));
//    QSqlQuery query;
//        query.prepare("SELECT nick FROM users WHERE nick = ?");
//        query.addBindValue(nick);
//        if(!query.exec() || query.first()){
//            return;
//        }
//        query.prepare("INSERT INTO users VALUES(?, ?)");
//        query.addBindValue(nick);
//        crypto.addData(pass.toStdString().c_str());
//        query.addBindValue(crypto.result().toHex().toStdString().c_str());
//        query.exec();
//        query.prepare("SELECT * FROM users WHERE nick = ? AND pass_hash = ?");

//        crypto.addData(pass.toStdString().c_str());
//        query.addBindValue(nick);
//        query.addBindValue(crypto.result().toHex().toStdString().c_str());
