/***************************************************************************
                          kmoptiondlg.h  -  description
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

#ifndef KMOPTIONDLG_H
#define KMOPTIONDLG_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmOptScans;
class KmOptDecoy;
class KmOptOther;
class KmOptStealth;
class KmOptAbout;
class KmOptPaths;

class KmOptionDlg : public QTabDialog  {
   Q_OBJECT
public: 
	KmOptionDlg(QWidget *parent=0, const char *name=0);
	~KmOptionDlg();

protected slots:
	void		slotApply();
	void		slotDefault();
	void		slotCancel();

private:
	KmOptScans *	opt_scans;
	KmOptOther *	opt_other;
	KmOptDecoy *	opt_decoy;
	KmOptStealth *	opt_stealth;
	KmOptAbout *	opt_about;
	KmOptPaths *	opt_paths;

signals:
	void		sigReloadArguments();
	
};

#endif
