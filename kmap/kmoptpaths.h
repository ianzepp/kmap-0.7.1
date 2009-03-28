/***************************************************************************
                          kmoptpaths.h  -  description
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

#ifndef KMOPTPATHS_H
#define KMOPTPATHS_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmOptPaths : public QWidget  {
   Q_OBJECT
public: 
	KmOptPaths(QWidget *parent=0, const char *name=0);
	~KmOptPaths();

	QString		getArguments();

	void		Load();
	void		LoadDefaults();

protected slots:
	virtual void	paintEvent( QPaintEvent * );

	void		slotUpdate();
	void		slotNmapBtn();
	void		slotHlogBtn();
	void		slotMlogBtn();

private:
	QLabel *	nmapLabel;
	QLineEdit *	nmapEdit;
	QPushButton *	nmapBtn;

	QCheckBox *	hlogBox;
	QLineEdit *	hlogEdit;
	QPushButton *	hlogBtn;

	QCheckBox *	mlogBox;
	QLineEdit *	mlogEdit;
	QPushButton *	mlogBtn;
};

#endif
