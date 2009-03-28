/***************************************************************************
                          kmhistory.h  -  description
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

#ifndef KMHISTORY_H
#define KMHISTORY_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmHistoryWidget; // forward

class KmHistory : public QTabDialog  {
   Q_OBJECT
public: 
	KmHistory(QWidget *parent=0, const char *name=0);
	~KmHistory();

	void		Load();
	void		LoadDefaults();

protected slots:
	void		slotApply();
	void		slotCancel();

private:
	KmHistoryWidget *kmhist;

signals:
	void		sigReloadHostname();
	void		sigReloadArguments();
};

class KmHistoryWidget : public QWidget  {
   Q_OBJECT
public:
	KmHistoryWidget(QWidget *parent=0, const char *name=0);
	~KmHistoryWidget();

	void		Load();
	void		LoadDefaults();
	void		Save();

protected slots:
	virtual void	paintEvent(QPaintEvent *);
	void		slotSelection(QListViewItem *);
	void		slotDoubleClicked(QListViewItem *);

	void		slotRemoveBtn();
	void		slotClearBtn();

protected:

private:
	QListView *	histList;
	QPushButton *	removeBtn;
	QPushButton *	clearBtn;

signals:
	void		sigReloadHostname();
	void		sigReloadArguments();
	void		sigAccept();
};

#endif
