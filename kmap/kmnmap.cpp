/***************************************************************************
                          kmnmap.cpp  -  description
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

#include "kmnmap.h"

KmNmap::KmNmap() : QObject()
{
	Reset();

	connect(&proc, SIGNAL(receivedStdout(KProcess *,char *,int)),
		 this, SLOT(slotStdout(KProcess *,char *,int)));
	connect(&proc, SIGNAL(receivedStderr(KProcess *,char *,int)),
		 this, SLOT(slotStderr(KProcess *,char *,int)));
	connect(&proc, SIGNAL(processExited(KProcess *)),
		 this, SLOT(slotExited(KProcess *)));
}
KmNmap::~KmNmap(){
}

int KmNmap::exitStatus()
{
	return proc.exitStatus();
}

bool KmNmap::isRunning()
{
	return proc.isRunning();
}

void KmNmap::Start()
{
	if (s_command.isEmpty())
	{
		QString tmp =	"Error: unable to start nmap: no command given!";
		emit sigStdout(tmp);
		return;
	}

	proc.clearArguments();
	
	// Add in arguments
	QTextStream ts(s_command, IO_ReadOnly);
	QString tmpstring;

	while (!ts.eof()) {
		ts >> tmpstring;
		proc << tmpstring;
	}

	emit sigStarted();

	if (proc.start(KProcess::NotifyOnExit, KProcess::AllOutput) == false)
	{
		QString tmp =	"Error: unable to start nmap!";
		emit sigStdout(tmp);
	}
}

void KmNmap::Stop()
{
	if (proc.kill() == false)
	{
		QString tmp =	"Error: unable to stop nmap!";
		emit sigStdout(tmp);
	}
}

void KmNmap::Reset()
{
	if (proc.isRunning()) {
		QString tmp =	">> Error: old nmap process is still running.\n"
				">> This will be killed now.";
		emit sigStdout(tmp);

		Stop();		
	}

	proc.clearArguments();

	s_command = "";

	s_pout = "";
	s_perr = "";

}
void KmNmap::setCommand(const QString &text)
{
	s_command = text;
}

void KmNmap::slotStdout(KProcess *p, char *buffer, int buflen)
{
	QString line;
	QString text = buffer;
	text = text.left(buflen);

	// Prepend partial if it exists
	if (!s_pout.isEmpty())
		text = s_pout + text;

	int pos = -1;

	while ((pos = text.find("\n")) != -1)
	{
		line = text.left(pos);
		text = text.remove(0, pos + 1);

		emit sigStdout(line);
	}

	if (!text.isEmpty())
		s_pout = text;
	else
		s_pout = "";
}
void KmNmap::slotStderr(KProcess *p, char *buffer, int buflen)
{
	QString line;
	QString text = buffer;
	text = text.left(buflen);

	// Prepend partial if it exists
	if (!s_perr.isEmpty())
		text = s_perr + text;

	int i;

	while ((i = text.find("\n")) != -1)
	{
		line = text.left(i);
		text = text.mid(i + 1, text.length());

		emit sigStderr(line);
	}

	if (!text.isEmpty())
		s_perr = text;
	else
		s_perr = "";
}

void KmNmap::slotExited(KProcess *p)
{
	// Flush out buffers
	if (!s_perr.isEmpty())
		emit sigStderr(s_perr);
	if (!s_pout.isEmpty())
		emit sigStdout(s_pout);
		
	// Check status
	QString tmp;

	if (p->normalExit())
		if (p->exitStatus())
			tmp = "Nmap: exited with errors";
		else
			tmp = "Nmap: exited normally";
	else
		tmp = "Nmap: exited with unknown errors";

	emit sigStdout(tmp);
	emit sigExited();
}















