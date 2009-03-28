/***************************************************************************
                          kmoptdecoy.h  -  description
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

#ifndef KMOPTDECOY_H
#define KMOPTDECOY_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmOptDecoy : public QWidget  {
   Q_OBJECT
public: 
	KmOptDecoy(QWidget *parent=0, const char *name=0);
	~KmOptDecoy();

	QString		getArguments();

	void		Load();
	void		LoadDefaults();

protected slots:
	virtual void	paintEvent( QPaintEvent * );

	void		slotUpdate();

	void		slotHighlighted( int );
	void		slotSelected( int );

	void		slotAdd();
	void		slotDel();
	void		slotUp();
	void		slotDown();

private:
	QCheckBox *	enableCheck;

	QLineEdit *	editBox;
	QListBox *	listBox;

	QPushButton *	addBtn;
	QPushButton *	delBtn;
	QPushButton *	upBtn;
	QPushButton *	downBtn;
};

#endif
