/***************************************************************************
                          kmap.cpp  -  description
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

#include "kmap.h"

#include "kmapview.h"
#include "kmportview.h"
#include "kminfoview.h"
#include "kmnmapview.h"
#include "kmnmap.h"
#include "kmconfig.h"
#include "kmoptiondlg.h"
#include "kmhistory.h"

Kmap::Kmap(QWidget *parent, const char *name) : KTMainWindow(name)
{
	KmConfig kmconf;

#if KDE_VERSION_MAJOR >= 2 || KDE_VERSION_MINOR > 5
	QPixmap icon_file_start(BarIcon(QString("forward.xpm")));
	QPixmap icon_file_stop(BarIcon(QString("stop.xpm")));
	QPixmap icon_view_options(BarIcon(QString("configure.xpm")));
	QPixmap icon_view_history(BarIcon(QString("contents.xpm")));
	QPixmap icon_view_port(BarIcon(QString("find.xpm")));
	QPixmap icon_view_info(BarIcon(QString("knife.xpm")));
	QPixmap icon_view_nmap(BarIcon(QString("leftjust.xpm")));
	QPixmap icon_file_quit(BarIcon(QString("delete.xpm")));
#else
	QPixmap icon_file_start(Icon(QString("forward.xpm")));
	QPixmap icon_file_stop(Icon(QString("stop.xpm")));
	QPixmap icon_view_options(Icon(QString("configure.xpm")));
	QPixmap icon_view_history(Icon(QString("contents.xpm")));
	QPixmap icon_view_port(Icon(QString("find.xpm")));
	QPixmap icon_view_info(Icon(QString("knife.xpm")));
	QPixmap icon_view_nmap(Icon(QString("leftjust.xpm")));
	QPixmap icon_file_quit(Icon(QString("delete.xpm")));
#endif
	//
	// Create menubar
	//
	fileMenu = new QPopupMenu();
	fileMenu->insertItem(icon_file_start, i18n("Start Scanning"), ID_FILE_START);
	fileMenu->insertItem(icon_file_stop, i18n("Stop Scanning"), ID_FILE_STOP);
	fileMenu->insertSeparator();
	fileMenu->insertItem(i18n("Quit"), ID_FILE_QUIT);

//	menuBar()->insertItem(i18n("&File"), fileMenu);

	connect(fileMenu, SIGNAL(activated(int)), SLOT(slotCommandCallback(int)));
	connect(fileMenu, SIGNAL(highlighted(int)), SLOT(slotStatusCallback(int)));

	//
	// Create toolbar
	//
	toolBar()->insertButton(icon_file_start, ID_FILE_START, true, i18n("Start Scanning"));
	toolBar()->insertButton(icon_file_stop, ID_FILE_STOP, true, i18n("Stop Scanning"));
	toolBar()->insertSeparator();
	toolBar()->insertButton(icon_view_options, ID_VIEW_OPTIONS, true, i18n("Setup Options"));
	toolBar()->insertButton(icon_view_history, ID_VIEW_HISTORY, true, i18n("History Window"));
	toolBar()->insertSeparator();
	toolBar()->insertButton(icon_view_port, ID_VIEW_PORT, true, i18n("Scanned Port View"));
	toolBar()->insertButton(icon_view_info, ID_VIEW_INFO, true, i18n("Information View"));
	toolBar()->insertButton(icon_view_nmap, ID_VIEW_NMAP, true, i18n("Nmap Output View"));
	toolBar()->insertSeparator();
	toolBar()->insertButton(icon_file_quit, ID_FILE_QUIT, true, i18n("Exit now"));
	
	// Set toggle buttons
	toolBar()->setToggle(ID_VIEW_PORT, true);
	toolBar()->setToggle(ID_VIEW_INFO, true);
	toolBar()->setToggle(ID_VIEW_NMAP, true);

	connect(toolBar(), SIGNAL(clicked(int)), SLOT(slotCommandCallback(int)));
	connect(toolBar(), SIGNAL(pressed(int)), SLOT(slotStatusCallback(int)));

	//
	// Create statusbar
	//
	statusBar()->message(i18n("Ready"));

	//
	// Check dir paths
	//
	updateDirs();

	//
	// try to find nmap
	//
	locateNmap();

	//
	// Create main view and set active view
	//
	km_view = new Kmapview(this);
	setView(km_view);

	connect(km_view, SIGNAL(sigPleaseStart()), SLOT(slotFileStart()) );

	//
	// Create port, info, and nmap views (all hidden)
	//
	km_port = new KmPortView(0);
	km_port->resize(kmconf.getPortViewSize());

	km_info = new KmInfoView(0);
	km_info->resize(kmconf.getInfoViewSize());

	km_nmap = new KmNmapView(0);
	km_nmap->resize(kmconf.getNmapViewSize());

	// Connect hideme sigs to slots
	connect(km_port, SIGNAL(sigHidden()), SLOT(slotPortHidden()) );
	connect(km_info, SIGNAL(sigHidden()), SLOT(slotInfoHidden()) );
	connect(km_nmap, SIGNAL(sigHidden()), SLOT(slotNmapHidden()) );

	//
	// Enable and disable as needed
	//
	disableCommand(ID_FILE_STOP);

	//
	// Toggle default toolbars
	//
	if (kmconf.getPortToggled())
	{
		toolBar()->toggleButton(ID_VIEW_PORT);
		km_port->show();
	}

	if (kmconf.getInfoToggled())
	{
		toolBar()->toggleButton(ID_VIEW_INFO);
		km_info->show();
	}

	if (kmconf.getNmapToggled())
	{
		toolBar()->toggleButton(ID_VIEW_NMAP);
		km_nmap->show();
	}

	//
	// Create process and connect signals
	//
	km_proc = new KmNmap();
	
	// Connect to here
	connect(km_proc, SIGNAL(sigStarted()), SLOT(slotStarted()) );
	connect(km_proc, SIGNAL(sigStdout(const QString &)), SLOT(slotStdout(const QString &)) );
	connect(km_proc, SIGNAL(sigStderr(const QString &)), SLOT(slotStderr(const QString &)) );
	connect(km_proc, SIGNAL(sigExited()), SLOT(slotExited()) );

	// Connect to main view
	connect(km_proc, SIGNAL(sigStarted()),
		km_view, SLOT(slotStarted()) );
	connect(km_proc, SIGNAL(sigExited()),
		km_view, SLOT(slotExited()) );

	// Connect to port
	connect(km_proc, SIGNAL(sigStarted()),
		km_port, SLOT(slotStarted()) );
	connect(km_proc, SIGNAL(sigStdout(const QString &)),
		km_port, SLOT(slotStdout(const QString &)) );
	connect(km_proc, SIGNAL(sigStderr(const QString &)),
		km_port, SLOT(slotStderr(const QString &)) );
	connect(km_proc, SIGNAL(sigExited()),
		km_port, SLOT(slotExited()) );

	// Connect to info
	connect(km_proc, SIGNAL(sigStarted()),
		km_info, SLOT(slotStarted()) );
	connect(km_proc, SIGNAL(sigStdout(const QString &)),
		km_info, SLOT(slotStdout(const QString &)) );
	connect(km_proc, SIGNAL(sigStderr(const QString &)),
		km_info, SLOT(slotStderr(const QString &)) );
	connect(km_proc, SIGNAL(sigExited()),
		km_info, SLOT(slotExited()) );

	// Connect to nmap out
	connect(km_proc, SIGNAL(sigStarted()),
		km_nmap, SLOT(slotStarted()) );
	connect(km_proc, SIGNAL(sigStdout(const QString &)),
		km_nmap, SLOT(slotStdout(const QString &)) );
	connect(km_proc, SIGNAL(sigStderr(const QString &)),
		km_nmap, SLOT(slotStderr(const QString &)) );
	connect(km_proc, SIGNAL(sigExited()),
		km_nmap, SLOT(slotExited()) );

	//
	// now get version
	//
	getNmapVersion();
}

Kmap::~Kmap()
{
	if (km_proc->isRunning())
		km_proc->Stop();

	KmConfig kmconf;

	// Save sizes
	kmconf.setPortViewSize(km_port->size());
	kmconf.setInfoViewSize(km_info->size());
	kmconf.setNmapViewSize(km_nmap->size());

	// Save toolbars
	kmconf.setPortToggled(toolBar()->isButtonOn(ID_VIEW_PORT));
	kmconf.setInfoToggled(toolBar()->isButtonOn(ID_VIEW_INFO));
	kmconf.setNmapToggled(toolBar()->isButtonOn(ID_VIEW_NMAP));

}

/*
 * Enable a command by it's id
 */
void Kmap::enableCommand(int id)
{
	menuBar()->setItemEnabled(id, true);
	toolBar()->setItemEnabled(id, true);
}

/*
 * Disable a command by it's id
 */
void Kmap::disableCommand(int id)
{
	menuBar()->setItemEnabled(id, false);
	toolBar()->setItemEnabled(id, false);
}

/*
 * Search for and locate nmap on the system
 */
void Kmap::locateNmap()
{
	KmConfig kmconf;
	QFileInfo finfo;
	QString path = kmconf.getNmapExecutable();

	finfo.setFile( path );
	if ( finfo.isExecutable() )
		return;

	QStrList list;
	QString text = "";

	list.append( text = "/bin/nmap" );
	list.append( text = "/sbin/nmap" );
	list.append( text = "/usr/bin/nmap" );
	list.append( text = "/usr/sbin/nmap" );
	list.append( text = "/usr/local/bin/nmap" );
	list.append( text = "/usr/local/sbin/nmap" );
	list.append( text = QDir::homeDirPath() + "/bin/nmap" ); // possibly
	list.append( text = "/opt/bin/nmap" ); // ???
	list.append( text = "/opt/sbin/nmap" ); // ???

	for ( uint i = 0 ; i < list.count() ; i++ )
	{
		path = list.at(i);

		finfo.setFile( path );

		if ( finfo.isExecutable() )
		{
			kmconf.setNmapExecutable(path);
			return;
		}
	}

	QMessageBox::information( this , "Kmap - Nmap not found" ,
				"I looked all over for Nmap, and I just couldn't find\n"
				"it installed anywhere. I rooted all through bin/ and\n"
				"sbin/ in all the system, local, and home directories,\n"
				"and it just wasn't there!\n"
				"\n"
				"You will need to open the setup dialog and set the\n"
				"correct path manually, since I sure can't find it. Once\n"
				"you do that, you won't need to again.\n"
				"\n"
				"If you forgot to install nmap, you can get it here:\n"
				"\n"
				"http://www.insecure.org/nmap\n"	
				);
}

/*
 * Asks nmap for it's version number
 */
void Kmap::getNmapVersion()
{
	in_version = true;

	// Feed to nmap
	KmConfig kmconf;
	QString tmp;

	// Clear any old version
	kmconf.setNmapVersion(tmp);

	// Now get new version
	tmp = kmconf.getNmapExecutable() + " --version";
	km_proc->setCommand(tmp);
	km_proc->Start();
}

/*
 * Check for common dir paths
 */
void Kmap::updateDirs()
{
	QString path;
	QDir dir;

	// Check basic kde dir;
}

/*
 * Runs a command depending on the ID we get
 */
void Kmap::slotCommandCallback(int id)
{
	switch (id)
	{
	case ID_FILE_START:
		slotFileStart();
		break;

	case ID_FILE_STOP:
		slotFileStop();
		break;

	case ID_FILE_QUIT:
		slotFileQuit();
		break;

	// VIEW id's
	case ID_VIEW_PORT:
		slotViewPort();
		break;

	case ID_VIEW_INFO:
		slotViewInfo();
		break;

	case ID_VIEW_NMAP:
		slotViewNmap();
		break;

	case ID_VIEW_OPTIONS:
		slotViewOptions();
		break;

	case ID_VIEW_HISTORY:
		slotViewHistory();
		break;

	default:
		break;
	}

}

/*
 * Shows help in the statusbar
 */
void Kmap::slotStatusCallback(int id)
{
	switch (id)
	{
	default:
		break;
	}
}
/*
 * Starts nmap running
 */
void Kmap::slotFileStart()
{
	KmConfig kmconf;

	QString args = km_view->arguments().stripWhiteSpace();
	QString host = km_view->hostname().stripWhiteSpace();
	QString tmp;

	// Grab and save the current host and args
	kmconf.setArguments(args);
	kmconf.setHostname(host);

	// Feed to nmap
	tmp += kmconf.getNmapExecutable();
	tmp += " ";
	tmp += args;
	tmp += " ";
	tmp += host;

	km_proc->setCommand(tmp);
	km_proc->Start();

	// Save to history
	QStrList list = kmconf.getHistoryList();
	QString text = host + " " + args;
	list.append(text);

	kmconf.setHistoryList(list);
}

/*
 * Stops (kills) the running nmap
 */
void Kmap::slotFileStop()
{
	km_proc->Stop();
}


/*
 * Quits the program
 */
void Kmap::slotFileQuit()
{
	this->close();
}

/*
 * Hide or show the port view
 */
void Kmap::slotViewPort()
{
	if (km_port->isVisible())
		km_port->hide();
	else
		km_port->show();
}

/*
 * Hide or show the info view
 */
void Kmap::slotViewInfo()
{
	if (km_info->isVisible())
		km_info->hide();
	else
		km_info->show();
}

/*
 * Hide or show the nmap view
 */
void Kmap::slotViewNmap()
{
	if (km_nmap->isVisible())
		km_nmap->hide();
	else
		km_nmap->show();
}

/*
 * Open the options dialog
 */
void Kmap::slotViewOptions()
{
	// Save current options in frontend
	KmConfig kmconf;
	kmconf.setArguments(km_view->arguments());

	// Show options
	KmOptionDlg *dlg = new KmOptionDlg(0);
	dlg->setCaption(i18n("Kmap: Setup Options"));

	// Connect reload sig
	connect(dlg, SIGNAL(sigReloadArguments()),
		km_view, SLOT(slotReloadArguments()) );

	dlg->show();

	// Delete it
	delete dlg;
}

/*
 * Open the history window
 */
void Kmap::slotViewHistory()
{
	KmHistory *dlg = new KmHistory(0);
	dlg->setCaption(i18n("Kmap: History Listing"));

	// Connect reload sig
	connect(dlg, SIGNAL(sigReloadHostname()),
		km_view, SLOT(slotReloadHostname()) );
	connect(dlg, SIGNAL(sigReloadArguments()),
		km_view, SLOT(slotReloadArguments()) );

	dlg->show();

	// Delete it
	delete dlg;
}

/*
 * Port window was hidden
 */
void Kmap::slotPortHidden()
{
	toolBar()->toggleButton(ID_VIEW_PORT);
}

/*
 * Info window was hidden
 */
void Kmap::slotInfoHidden()
{
	toolBar()->toggleButton(ID_VIEW_INFO);
}

/*
 * Nmap window was hidden
 */
void Kmap::slotNmapHidden()
{
	toolBar()->toggleButton(ID_VIEW_NMAP);
}

/*
 * Nmap has just started
 */
void Kmap::slotStarted()
{
	disableCommand(ID_FILE_START);
	disableCommand(ID_FILE_QUIT);
	disableCommand(ID_VIEW_OPTIONS);
	disableCommand(ID_VIEW_HISTORY);
	enableCommand(ID_FILE_STOP);
}

/*
 * Stdout text from nmap
 */
void Kmap::slotStdout(const QString &text)
{
	statusBar()->message(text);

	// In version?
	if (!in_version)
		return;

	if (text.find(QRegExp("^nmap V. [0-9a-zA-Z\\.]+")) == -1)
		return;

	KmConfig kmconf;
	kmconf.setNmapVersion(text.mid(8,text.length()));

	//
	// Set caption
	//
	QString tmp;
	tmp += i18n("Kmap network scanner");
	tmp += " [";
	tmp += text;
	tmp += "]";

	setCaption(tmp);

}

/*
 * Stdout text from nmap
 */
void Kmap::slotStderr(const QString &text)
{
	statusBar()->message(text);
}

/*
 * Nmap has just stopped
 */
void Kmap::slotExited()
{
	disableCommand(ID_FILE_STOP);
	enableCommand(ID_FILE_START);
	enableCommand(ID_FILE_QUIT);
	enableCommand(ID_VIEW_OPTIONS);
	enableCommand(ID_VIEW_HISTORY);

	if (!in_version)
		return;

	in_version = false;

	// Clear widgets
	km_port->Clear();
	km_info->Clear();
	km_nmap->Clear();

	statusBar()->message(i18n("Ready"));
}

/*
 * Empty slot for line sep
 */
void Kmap::slotLineSep()
{
}


