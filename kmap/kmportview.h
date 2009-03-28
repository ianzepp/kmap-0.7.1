/***************************************************************************
                          kmportview.h  -  description
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

#ifndef KMPORTVIEW_H
#define KMPORTVIEW_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmPortView : public QWidget  {
   Q_OBJECT
public: 
	KmPortView(QWidget *parent=0, const char *name=0);
	~KmPortView();

	void		Clear();

public slots:
	void		slotStarted();
	void		slotStdout( const QString & );
	void		slotStderr( const QString & );
	void		slotExited();

protected slots:
	void		slotLogFile();
	void		slotBtnBrowse();
	void		slotBtnSave();

	virtual void	paintEvent( QPaintEvent * );
	virtual void	closeEvent( QCloseEvent * );

protected:
	void		handleIpAddress( const QString & );
	void		handlePingState( const QString & , bool );
	void		handlePortEntry( const QString & );

private:
	QListView *	mainView;
	QString		current_ip;

	bool 		in_port_list;
	bool 		is_ping_scan;

	QCheckBox *	saveBox;
	QLineEdit *	pathEdit;
	QPushButton *	browseBtn;
	QPushButton *	saveBtn;

signals:
	void		sigHidden();
};

#endif
