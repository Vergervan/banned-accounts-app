#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QObject>
#include <QTextStream>

class Configurator : public QObject
{
    Q_OBJECT
    public:
        struct ConfigInfo{
            bool newConfig = false;
            bool rememberUser = false;
            QString username, passhash;
        };
        Configurator();
        ConfigInfo getConfigInfo();
    //private:
    QString _configPath;
    std::map<QString, QString> _dict;
    public slots:
        void rememberUser(QString username, QString passhash);
};

Q_DECLARE_METATYPE(Configurator::ConfigInfo)

#endif // CONFIGURATOR_H
