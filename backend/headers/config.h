#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <QHostAddress>
#include <QFile>

struct Config
{
	QHostAddress address;
	int port;
};

Config readcfg(const char* filename)
{
	QFile file(filename);
	QString str;
	std::map<QString, QString> dict; 
	file.open(QIODevice::OpenMode::enum_type::ReadOnly);
	if(file.isOpen())
	{
		str = QString(file.readAll());	
	}
	auto list = str.split('\n');
	foreach(const auto& str, list)
	{
		if(str.isEmpty()) continue;
		auto keyval = str.split('=');
		dict.insert({ keyval[0], keyval[1] });
	}
	Config cfg;
	cfg.address = QHostAddress(dict["host"]);
	cfg.port = dict["port"].toInt();
	return cfg;
}
#endif