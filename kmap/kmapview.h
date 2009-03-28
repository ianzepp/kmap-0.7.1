/***************************************************************************
                          kmapview.h  -  description
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

#ifndef KMAPVIEW_H
#define KMAPVIEW_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class Kmapview : public QWidget  {
   Q_OBJECT
public: 
	Kmapview(QWidget *parent=0, const char *name=0);
	~Kmapview();

	QString 	hostname();
	QString 	arguments();

public slots:
	void		slotStarted();
	void		slotExited();
	void		slotReloadHostname();
	void		slotReloadArguments();

protected slots:
	virtual void	paintEvent( QPaintEvent * );

	void		slotHostReturn();
	void		slotArgsReturn();

private:
	// Host/args
	QLabel *	hostLabel;
	QLineEdit *	hostEdit;

	QLabel *	argsLabel;
	QLineEdit *	argsEdit;

signals:
	void		sigPleaseStart();
};

#endif
