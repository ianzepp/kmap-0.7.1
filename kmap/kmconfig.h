/***************************************************************************
                          kmconfig.h  -  description
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

#ifndef KMCONFIG_H
#define KMCONFIG_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

#define IDS_NMAP_EXECUTABLE	"Nmap_Executable"
#define IDS_NMAP_VERSION	"Nmap_Version"
#define IDS_ARGUMENTS		"Arguments"
#define IDS_HOSTNAME		"Hostname"
#define IDS_FTP_BOUNCE		"Ftp_Bounce"
#define IDS_PING_PORT		"Ping_Port"
#define IDS_PORT_RANGE		"Port_Range"
#define IDS_HUMAN_LOG_PATH	"Human_Log_Path"
#define IDS_MACHINE_LOG_PATH	"Machine_Log_Path"

#define IDS_PORT_VIEW_LOG	"Port_View_Log"
#define IDS_INFO_VIEW_LOG	"Info_View_Log"
#define IDS_NMAP_VIEW_LOG	"Nmap_View_Log"

#define IDS_PORT_VIEW_SIZE	"Port_View_Size"
#define IDS_INFO_VIEW_SIZE	"Info_View_Size"
#define IDS_NMAP_VIEW_SIZE	"Nmap_View_Size"

#define IDS_PORT_TOGGLED	"Port_Is_Toggled"
#define IDS_INFO_TOGGLED	"Info_Is_Toggled"
#define IDS_NMAP_TOGGLED	"Nmap_Is_Toggled"

class KmConfig {
public: 
	KmConfig();
	~KmConfig();

	// Decide whether a root user or not
	static bool	isRootUser();

	// Saved text
	QString		getNmapExecutable();
	void		setNmapExecutable(const QString &);

	QString		getNmapVersion();
	void		setNmapVersion(const QString &);

	QString		getArguments();
	void		setArguments(const QString &);

	QString		getHostname();
	void		setHostname(const QString &);

	QString		getFtpBounceHost();
	void		setFtpBounceHost(const QString &);

	QString		getUsePingPort();
	void		setUsePingPort(const QString &);

	QString		getPortRange();
	void		setPortRange(const QString &);

	QString		getHumanLogPath();
	void		setHumanLogPath(const QString &);

	QString		getPortViewLog();
	void		setPortViewLog(const QString &);

	QString		getInfoViewLog();
	void		setInfoViewLog(const QString &);

	QString		getNmapViewLog();
	void		setNmapViewLog(const QString &);

	QString		getMachineLogPath();
	void		setMachineLogPath(const QString &);

	static QStrList	getHistoryList();
	static void	setHistoryList(QStrList &);

	static QString	getFingerPrints();
	static void	appendFingerPrint(const QString &);

	// Sizes
	QSize		getPortViewSize();
	void		setPortViewSize(const QSize &);

	QSize		getInfoViewSize();
	void		setInfoViewSize(const QSize &);

	QSize		getNmapViewSize();
	void		setNmapViewSize(const QSize &);

	// Toggle buttons
	bool		getPortToggled();
	void		setPortToggled(bool);

	bool		getInfoToggled();
	void		setInfoToggled(bool);

	bool		getNmapToggled();
	void		setNmapToggled(bool);

	// Global coords
	QPoint		getGlobalKmap();
	void		setGlobalKmap(const QPoint &);

private:
	KConfig *	config;
};

#endif
