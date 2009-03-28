/***************************************************************************
                          kmap.h  -  description
                             -------------------
    begin                : Fri Jan  7 00:40:23 EST 2000
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

#ifndef KMAP_H
#define KMAP_H

#include "kmcommon.h"

#define ID_FILE_QUIT		1000
#define ID_FILE_START		1001
#define ID_FILE_STOP		1002

#define ID_VIEW_PORT		1101
#define ID_VIEW_INFO		1102
#define ID_VIEW_NMAP		1103
#define ID_VIEW_OPTIONS		1110
#define ID_VIEW_HISTORY		1111

#define ID_LINE_SEP		8002 // just cause

class Kmapview;	// forward: for main view
class KmNmap;		// forward: for the process
class KmPortView;	// forward: for the port scanning view
class KmInfoView;	// forward: for the informational view
class KmNmapView;	// forward: for the nmap output view

class Kmap : public KTMainWindow
{
Q_OBJECT
public:
	/** construtor */
	Kmap(QWidget* parent=0, const char *name=0);
	/** destructor */
	~Kmap();

protected:
	void		enableCommand(int);
	void		disableCommand(int);

	// Find out where nmap is
	void		locateNmap();
	void		getNmapVersion();

	// Check dir paths
	void		updateDirs();

protected slots:
	void		slotCommandCallback(int);
	void		slotStatusCallback(int);

	void		slotFileStart();
	void		slotFileStop();
	void		slotFileQuit();

	void		slotViewPort();
	void		slotViewInfo();
	void		slotViewNmap();

	void		slotViewOptions();
	void		slotViewHistory();

	// Windows were hidden
	void		slotPortHidden();
	void		slotInfoHidden();
	void		slotNmapHidden();

	// Nmap has started/stopped
	void		slotStarted();
	void		slotStdout( const QString & );
	void		slotStderr( const QString & );
	void		slotExited();

	// Empty slot for lined spe
	void		slotLineSep();

private:
	QPopupMenu *	fileMenu;
	QPopupMenu *	helpMenu;

	Kmapview *	km_view;
	KmNmap *	km_proc;
	KmPortView *	km_port;
	KmInfoView *	km_info;
	KmNmapView *	km_nmap;

	bool		in_version;
};

#endif
