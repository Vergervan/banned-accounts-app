#include "authorizationmanager.h"

void AuthorizationManager::auth(QString nick, QString pass, bool reg)
{
    QCryptographicHash crypto(QCryptographicHash::Sha256);

    QSqlQuery query;
    if(reg){
        query.prepare("SELECT nick FROM users WHERE nick = ?");
        query.addBindValue(nick);
        if(!query.exec() || query.first()){

            emit sendAuthResult(false);
            return;
        }
        query.prepare("INSERT INTO users VALUES(?, ?)");
        query.addBindValue(nick);
        crypto.addData(pass.toStdString().c_str());
        query.addBindValue(crypto.result().toHex().toStdString().c_str());
        query.exec();
        emit sendAuthResult(true);
        return;
    }else{
        query.prepare("SELECT * FROM users WHERE nick = ? AND pass_hash = ?");

        crypto.addData(pass.toStdString().c_str());
        query.addBindValue(nick);
        query.addBindValue(crypto.result().toHex().toStdString().c_str());
        if(!query.exec() || !query.first()){
            emit sendAuthResult(false);
            return;
        }
        emit sendAuthResult(true);
    }
}

AuthorizationManager::AuthorizationManager()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sprawl_prod.db");
    db.open();
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS users(nick TEXT PRIMARY KEY, pass_hash TEXT NOT NULL)");
}
