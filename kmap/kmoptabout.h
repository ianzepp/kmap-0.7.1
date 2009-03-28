/***************************************************************************
                          kmoptabout.h  -  description
                             -------------------
    begin                : Sat Jan 8 2000
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

#ifndef KMOPTABOUT_H
#define KMOPTABOUT_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmOptAbout : public QWidget  {
   Q_OBJECT
public: 
	KmOptAbout(QWidget *parent=0, const char *name=0);
	~KmOptAbout();

	void		LoadDefaults();
	void		Load();

protected slots:
	virtual void	paintEvent(QPaintEvent *);

private:
	QLabel *	countLabel;
	QLabel *	timeLabel;
	QLabel *	histLabel;
	QLabel *	logsLabel;

	QLabel *	aboutLabel;
};

#endif
