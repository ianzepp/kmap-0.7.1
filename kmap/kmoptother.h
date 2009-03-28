/***************************************************************************
                          kmoptother.h  -  description
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

#ifndef KMOPTOTHER_H
#define KMOPTOTHER_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmOptOther : public QWidget  {
   Q_OBJECT
public: 
	KmOptOther(QWidget *parent=0, const char *name=0);
	~KmOptOther();

	QString 	getArguments();

	void		Load();
	void		LoadDefaults();

protected slots:
	virtual void	paintEvent( QPaintEvent * );

	void		slotUpdate();

	void		slotFastNotPort();
	void		slotPortNotFast();

	void		slotRonNotRoff();
	void		slotRoffNotRon();

private:
	QCheckBox *	osidBox;
	QCheckBox *	identBox;
	QCheckBox *	fastBox;
	QCheckBox *	portBox;
	QCheckBox *	fragBox;
	QCheckBox *	randBox;	// randomize
	QCheckBox *	roffBox;	// never resolv
	QCheckBox *	ronBox;		// always resolv

	QCheckBox *	verboseBox;
	QCheckBox *	debugBox;

	QComboBox *	verboseLvlBox;
	QComboBox *	debugLvlBox;

	QLineEdit *	portEdit;
};

#endif
