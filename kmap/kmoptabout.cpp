/***************************************************************************
                          kmoptabout.cpp  -  description
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

#include "kmoptabout.h"

KmOptAbout::KmOptAbout(QWidget *parent, const char *name ) : QWidget(parent,name) {
	countLabel = new QLabel(i18n("Scanned:"), this);
	timeLabel = new QLabel(i18n("Time:"), this);
	histLabel = new QLabel(i18n("Entries:"), this);
	logsLabel = new QLabel(i18n("Logs:"), this);

	QString tmp = i18n("Kmap Network Scanner\n"
			"\n"
			"Kmap is a KDE frontend to the network scanner nmap.\n"
			"Nmap is THE most advanced scanner (to my knowledge)\n"
			"available at this time, and support over 6 major\n"
			"scan types with a variety of supporting options.\n"
			"\n"
			"Kmap was written by: Ian Zepp <icszepp@islc.net>\n"
			"\n"
			"Kmap is at http://www.islc.net/~icszepp/kmap.html\n"
			"Nmap is at http://www.insecure.org/nmap\n"
			);

	aboutLabel = new QLabel(tmp, this);
	aboutLabel->setAlignment(AlignTop);

	LoadDefaults();
	Load();	
}
KmOptAbout::~KmOptAbout(){
}

/*
 *
 */
void KmOptAbout::LoadDefaults()
{
}

/*
 *
 */
void KmOptAbout::Load()
{

}

/*
 *
 */
void KmOptAbout::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	int w_left = 120;

	countLabel->setGeometry(10, 10, w_left, 20);
	timeLabel->setGeometry(10, 30, w_left, 20);
	histLabel->setGeometry(10, 50, w_left, 20);
	logsLabel->setGeometry(10, 70, w_left, 20);

	aboutLabel->setGeometry(w_left + 20, 10, width() - w_left - 30, height() - 20);
}
