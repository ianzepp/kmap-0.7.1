/***************************************************************************
                          kmnmapview.h  -  description
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

#ifndef KMNMAPVIEW_H
#define KMNMAPVIEW_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmNmapView : public QWidget  {
   Q_OBJECT
public: 
	KmNmapView(QWidget *parent=0, const char *name=0);
	~KmNmapView();

	void		Clear();
	void		Append( const QString & );

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

private:
	QMultiLineEdit *textView;

	QCheckBox *	saveBox;
	QLineEdit *	pathEdit;
	QPushButton *	browseBtn;
	QPushButton *	saveBtn;

signals:
	void		sigHidden();
};

#endif
