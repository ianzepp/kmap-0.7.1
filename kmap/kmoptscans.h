/***************************************************************************
                          kmoptscans.h  -  description
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

#ifndef KMOPTSCANS_H
#define KMOPTSCANS_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmOptScans : public QWidget  {
   Q_OBJECT
public: 
	KmOptScans(QWidget *parent=0, const char *name=0);
	~KmOptScans();

	QString 	getArguments();

	void		Load();
	void		LoadDefaults();

protected slots:
	virtual void	paintEvent( QPaintEvent * );

	void		slotUpdate();

protected:
	void		slotNormal( int );
	void		slotRoot( int );
	void		slotBounce();

private:
	QButtonGroup *	scanGrp;

	QRadioButton *	normalRadio;
	QRadioButton *	rootRadio;
	QRadioButton *	bounceRadio;

	QComboBox *	normalBox;
	QComboBox *	rootBox;
	QLineEdit *	bounceEdit;

	QLabel *	pingLabel;

	QComboBox *	pingBox;
	QCheckBox *	portBox;
	QCheckBox *	synBox;

	QLineEdit *	portEdit;
};

#endif
