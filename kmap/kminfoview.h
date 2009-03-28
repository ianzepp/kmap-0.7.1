/***************************************************************************
                          kminfoview.h  -  description
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

#ifndef KMINFOVIEW_H
#define KMINFOVIEW_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmInfoView : public QWidget  {
   Q_OBJECT
public: 
	KmInfoView(QWidget *parent=0, const char *name=0);
	~KmInfoView();

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
	void		handlePortEntry( const QString & );
	void		handlePrintEntry( const QString & );
	void		handleIpAddress( const QString & ); // normal scan
	void		handleIpAddress2( const QString & ); // ping scan

	void		handleClass( const QString & );
	void		handleDifficulty( const QString & );
	void		handleSequence( const QString & );
	void		handleRemoteOS( const QString & );
	void		handleUnknownOS( const QString & );

private:
	int 		i_cnt;
	bool		in_port_list;
	bool		in_fingerprint;

	QListView *	mainView;
	QPixmap *	pix;
	QListViewItem *	p_currentip;

	QString		current_ip;
	QString		current_host;
	QString		tcp_port_list;
	QString		udp_port_list;
	QString		misc_port_list;

	QString		our_problem;
	QString		our_solution;

	QString		print_text;

	QCheckBox *	saveBox;
	QLineEdit *	pathEdit;
	QPushButton *	browseBtn;
	QPushButton *	saveBtn;

signals:
	void		sigHidden();
};

#endif
