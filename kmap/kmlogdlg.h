/***************************************************************************
                          kmlogdlg.h  -  description
                             -------------------
    begin                : Sun Jan 9 2000
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

#ifndef KMLOGDLG_H
#define KMLOGDLG_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmLogDlg : public QWidget  {
   Q_OBJECT
public: 
	KmLogDlg(QWidget *parent=0, const char *name=0);
	~KmLogDlg();

	void		LoadDefaults();
	void		Load();

protected slots:
	void		slotPortLogFile();
	void		slotPortBtnBrowse();
	void		slotPortBtnSave();

	void		slotInfoLogFile();
	void		slotInfoBtnBrowse();
	void		slotInfoBtnSave();

	void		slotNmapLogFile();
	void		slotNmapBtnBrowse();
	void		slotNmapBtnSave();

	virtual void	paintEvent( QPaintEvent * );

private:
	// Port widgets
	QCheckBox *	savePortBox;
	QLineEdit *	pathPortEdit;
	QPushButton *	browsePortBtn;
	QPushButton *	savePortBtn;

	// Info Widgets
	QCheckBox *	saveInfoBox;
	QLineEdit *	pathInfoEdit;
	QPushButton *	browseInfoBtn;
	QPushButton *	saveInfoBtn;

	// Nmap Widgets
	QCheckBox *	saveNmapBox;
	QLineEdit *	pathNmapEdit;
	QPushButton *	browseNmapBtn;
	QPushButton *	saveNmapBtn;

signals:
	void		sigSavePort();
	void		sigSaveInfo();
	void		sigSaveNmap();
};

#endif
