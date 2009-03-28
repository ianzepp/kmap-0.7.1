/***************************************************************************
                          kmoptstealth.h  -  description
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

#ifndef KMOPTSTEALTH_H
#define KMOPTSTEALTH_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

// TODO: reformat member names

class KmOptStealth : public QWidget  {
   Q_OBJECT
public: 
	KmOptStealth(QWidget *parent=0, const char *name=0);
	~KmOptStealth();

	QString		getArguments();

	void		Load();
	void		LoadDefaults();

protected slots:
	virtual void	paintEvent( QPaintEvent * );

	void		slotUpdate();

	void		slotHost();
	void		slotMaxRtt();
	void		slotInitRtt();
	void		slotParallel();
	void		slotDelay();

private:
	QCheckBox *	btn_nmapdef;
	QCheckBox *	btn_userdef;

	QComboBox *	w_canned;

	QCheckBox *	w_host;
	QCheckBox *	w_max_rtt;
	QCheckBox *	w_init_rtt;
	QCheckBox *	w_parallel;
	QCheckBox *	w_delay;

	QLineEdit *	w_host_text;
	QLineEdit *	w_max_rtt_text;
	QLineEdit *	w_init_rtt_text;
	QLineEdit *	w_parallel_text;
	QLineEdit *	w_delay_text;
};

#endif
