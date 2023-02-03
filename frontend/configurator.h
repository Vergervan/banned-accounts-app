#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QObject>
#include <QTextStream>
#include "Windows.h"
#include <QCoreApplication>
#include <string>
#include <sstream>

class Configurator : public QObject
{
    Q_OBJECT
    public:
        struct ConfigInfo{
            bool newConfig = false;
            bool autostart = false;
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
        void setAutostartApplication(bool);
    private:
        std::wstring widen(const std::string& str);
};

Q_DECLARE_METATYPE(Configurator::ConfigInfo)

#endif // CONFIGURATOR_H
