/***************************************************************************
                          kmnmap.h  -  description
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

#ifndef KMNMAP_H
#define KMNMAP_H

#include "kmcommon.h"

/**
  *@author Ian Zepp
  */

class KmNmap : public QObject {
  Q_OBJECT
public:
	KmNmap();
	~KmNmap();

	int		exitStatus();
	bool		isRunning();

	void		Start();
	void		Stop();

	void		Reset();
	void		setCommand( const QString & );

private slots:

	void		slotStdout( KProcess * , char * , int );
	void		slotStderr( KProcess * , char * , int );
	void		slotExited( KProcess * );

private:
	KProcess	proc;

	QString		s_command;
	QString		s_pout;
	QString		s_perr;

signals:
	void		sigStarted();
	void		sigStdout( const QString & );
	void		sigStderr( const QString & );
	void		sigExited();
};

#endif
