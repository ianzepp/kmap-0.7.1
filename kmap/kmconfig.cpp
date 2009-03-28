/***************************************************************************
                          kmconfig.cpp  -  description
                             -------------------
    begin                : Fri Jan 7 2000
    copyright            : (C) 2000 by Ian Zepp
    email                : icszepp@islc.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kmconfig.h"

KmConfig::KmConfig(){
// Test for kde2
#if KDE_VERSION_MAJOR >= 2 || KDE_VERSION_MINOR > 5
	config = kapp->config();
#else
	config = kapp->getConfig();
#endif

}
KmConfig::~KmConfig(){
}

// Root user?
bool KmConfig::isRootUser()
{
	QFileInfo finfo(QDir::rootDirPath());

	if (finfo.isWritable())
		return true;
	else
		return false;
}

// Strings
QString KmConfig::getNmapExecutable()
{
	QString tmp = config->readEntry("Nmap_Executable");
	return tmp;
}
void KmConfig::setNmapExecutable(const QString &text)
{
	config->writeEntry("Nmap_Executable", text);
}

QString KmConfig::getNmapVersion()
{
	QString tmp = config->readEntry("Nmap_Version");
	return tmp;
}
void KmConfig::setNmapVersion(const QString &text)
{
	config->writeEntry("Nmap_Version", text);
}

QString KmConfig::getArguments()
{
	QString tmp = config->readEntry("Nmap_Arguments");
	return tmp;
}
void KmConfig::setArguments(const QString &text)
{
	config->writeEntry("Nmap_Arguments", text);
}

QString KmConfig::getHostname()
{
	QString tmp = config->readEntry("Nmap_Hostname");
	return tmp;
}
void KmConfig::setHostname(const QString &text)
{
	config->writeEntry("Nmap_Hostname", text);
}

QString KmConfig::getFtpBounceHost()
{
	QString tmp = config->readEntry("Ftp_Bounce_Host");
	return tmp;
}
void KmConfig::setFtpBounceHost(const QString &text)
{
	config->writeEntry("Ftp_Bounce_Host", text);
}

QString KmConfig::getUsePingPort()
{
	QString tmp = config->readEntry("Alt_Ping_Port");
	return tmp;
}
void KmConfig::setUsePingPort(const QString &text)
{
	config->writeEntry("Alt_Ping_Port", text);
}

QString KmConfig::getPortRange()
{
	QString tmp = config->readEntry("Specify_Port_Range");
	return tmp;
}
void KmConfig::setPortRange(const QString &text)
{
	config->writeEntry("Specify_Port_Range", text);
}

QString KmConfig::getHumanLogPath()
{
	QString tmp = config->readEntry("Log_Path_Human");
	return tmp;
}
void KmConfig::setHumanLogPath(const QString &text)
{
	config->writeEntry("Log_Path_Human", text);
}

QString KmConfig::getMachineLogPath()
{
	QString tmp = config->readEntry("Log_Path_Machine");
	return tmp;
}
void KmConfig::setMachineLogPath(const QString &text)
{
	config->writeEntry("Log_Path_Machine", text);
}

QString KmConfig::getPortViewLog()
{
	QString tmp = config->readEntry("Port_View_Log");
	return tmp;
}
void KmConfig::setPortViewLog(const QString &text)
{
	config->writeEntry("Port_View_Log", text);
}

QString KmConfig::getInfoViewLog()
{
	QString tmp = config->readEntry("Info_View_Log");
	return tmp;
}
void KmConfig::setInfoViewLog(const QString &text)
{
	config->writeEntry("Info_View_Log", text);
}

QString KmConfig::getNmapViewLog()
{
	QString tmp = config->readEntry("Nmap_View_Log");
	return tmp;
}
void KmConfig::setNmapViewLog(const QString &text)
{
	config->writeEntry("Nmap_View_Log", text);
}

QStrList KmConfig::getHistoryList()
{
	QString path = QDir::homeDirPath() + "/.kde/share/apps/kmap/history";

	QStrList tmp; // different format for reading lists
        QFile file(path);
	
	// Open file
	if (!file.open(IO_ReadOnly))
	{
		warning("Unable to open history file: %s", path.data() );
		return tmp;
	}

	// Read strings
	QTextStream is(&file);

	while (!is.eof())
		tmp.append(is.readLine());

	// return list, if any
	file.close();
	return tmp;
}
void KmConfig::setHistoryList(QStrList &list)
{
	QString path = QDir::homeDirPath() + "/.kde/share/apps/kmap/history";
        QFile file(path);
	
	// Open file
	if (!file.open(IO_WriteOnly | IO_Truncate))
	{
		warning("Unable to open history file: %s", path.data() );
		return;
	}

	// Write strings
	QString text;

	for (uint i = 0; i < list.count(); i++)
	{
		text += list.at(i);
		text += "\n";
	}

	if (text.isEmpty())
	{
		file.close(); // truncated
		return;
	}

	if (!file.writeBlock(text.data(), text.length()))
	{
		warning("Write error to history file: %s", path.data() );
		file.close();
		return;
	}

	file.close();
}

QString KmConfig::getFingerPrints()
{
	QString path = QDir::homeDirPath() + "/.kde/share/apps/kmap/fingerprints";

	QString tmp; // different format for reading lists
        QFile file(path);
	
	// Open file
	if (!file.open(IO_ReadOnly))
	{
		warning("Unable to open print file: %s", path.data() );
		return tmp;
	}

	// Read strings
	QTextStream is(&file);

	while (!is.eof())
	{
		tmp += is.readLine();
		tmp += "\n";
	}

	// return text
	file.close();
	return tmp;
}
void KmConfig::appendFingerPrint(const QString &text)
{
	QString path = QDir::homeDirPath() + "/.kde/share/apps/kmap/fingerprints";
        QFile file(path);
	
	// Open file
	if (!file.open(IO_WriteOnly | IO_Append))
	{
		warning("Unable to open print file: %s", path.data() );
		return;
	}

	if (text.isEmpty())
	{
		file.close(); // nothing to do
		return;
	}

	if (!file.writeBlock(text.data(), text.length()))
	{
		warning("Write error to print file: %s", path.data() );
		file.close();
		return;
	}

	file.close();
}

//
// Sizes
//
QSize KmConfig::getPortViewSize()
{
	QSize tmp = config->readSizeEntry("Size_Port_View", new QSize(500,250));
	return tmp;
}
void KmConfig::setPortViewSize(const QSize &size)
{
	config->writeEntry("Size_Port_View", size);
}

QSize KmConfig::getInfoViewSize()
{
	QSize tmp = config->readSizeEntry("Size_Info_View", new QSize(500,150));
	return tmp;
}
void KmConfig::setInfoViewSize(const QSize &size)
{
	config->writeEntry("Size_Info_View", size);
}

QSize KmConfig::getNmapViewSize()
{
	QSize tmp = config->readSizeEntry("Size_Nmap_View", new QSize(500,250));
	return tmp;
}
void KmConfig::setNmapViewSize(const QSize &size)
{
	config->writeEntry("Size_Nmap_View", size);
}

//
// Toggle buttons
//
bool KmConfig::getPortToggled()
{
	bool tmp = config->readBoolEntry("Bool_Port_Toggled", false);
	return tmp;
}
void KmConfig::setPortToggled(bool tmp)
{
	config->writeEntry("Bool_Port_Toggled", tmp);
}

bool KmConfig::getInfoToggled()
{
	bool tmp = config->readBoolEntry("Bool_Info_Toggled", false);
	return tmp;
}
void KmConfig::setInfoToggled(bool tmp)
{
	config->writeEntry("Bool_Info_Toggled", tmp);
}

bool KmConfig::getNmapToggled()
{
	bool tmp = config->readBoolEntry("Bool_Nmap_Toggled", false);
	return tmp;
}
void KmConfig::setNmapToggled(bool tmp)
{
	config->writeEntry("Bool_Nmap_Toggled", tmp);
}

//
// Global coords
//
QPoint KmConfig::getGlobalKmap()
{
	QPoint tmp = config->readPointEntry("Global_Kmap");
	return tmp;
}
void KmConfig::setGlobalKmap(const QPoint &tmp)
{
	config->writeEntry("Global_Kmap", tmp);
}
