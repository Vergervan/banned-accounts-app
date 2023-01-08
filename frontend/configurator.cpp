#include "configurator.h"

Configurator::Configurator(){
    _configPath = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0);
    QDir dir;
    if(!dir.exists(_configPath))
        dir.mkpath(_configPath);
    QFile cfgFile(_configPath + "/config.cfg");
    if(cfgFile.open(QFile::ReadOnly))
        cfgFile.close();
}

void Configurator::rememberUser(QString username, QString passhash)
{
    QFile cfgFile(_configPath + "/config.cfg");
    QTextStream stream(&cfgFile);
    stream.setEncoding(QStringConverter::Utf8);
    cfgFile.open(QFile::WriteOnly);
    QString filestr;
    if(cfgFile.isOpen())
        filestr = QString(cfgFile.readAll());
    if(filestr.isEmpty())
    {
        stream << "RememberUser=true\n";
        stream << QString("Username=%1\nPasshash=%2\n").arg(username, passhash);
        stream.flush();
        cfgFile.close();
        return;
    }
    QString newStr;
    auto list = filestr.split('\n');
    foreach(const auto& str, list)
    {
        if(str.isEmpty())
            newStr += str;
        else if(str.indexOf("RememberUser=") != -1)
            newStr += "RememberUser=true\n";
        else if(str.indexOf("Username=") != -1)
            newStr += QString("Username=%1\n").arg(username);
        else if(str.indexOf("Passhash=") != -1)
            newStr += QString("Passhash=%1\n").arg(passhash);
        else
            newStr += str;
    }
    cfgFile.resize(0);
    stream << newStr;
    stream.flush();
    cfgFile.close();
}

Configurator::ConfigInfo Configurator::getConfigInfo(){
    _dict.clear();
    ConfigInfo info;
    QFile cfgFile(this->_configPath + "/config.cfg");
    cfgFile.open(QFile::ReadOnly);
    QString filestr = QString(cfgFile.readAll());
    info.newConfig = filestr.isEmpty();
    if(!info.newConfig)
    {
        QList<QString> list = filestr.split('\n');
        foreach(const auto& str, list)
        {
            if(str.isEmpty()) continue;
            QStringList keyval = str.split('=');
            _dict.insert({ keyval[0], keyval[1] });
        }
        auto val = _dict.find("RememberUser");
        info.rememberUser = (val != _dict.end()) && (val->second == "true");
        if(info.rememberUser)
        {
            info.username = _dict["Username"], info.passhash = _dict["Passhash"];
        }
    }
    cfgFile.close();
    return info;
}
