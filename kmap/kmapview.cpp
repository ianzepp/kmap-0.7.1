/***************************************************************************
                          kmapview.cpp  -  description
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

#include "kmapview.h"
#include "kmconfig.h"

Kmapview::Kmapview(QWidget *parent, const char *name ) : QWidget(parent,name) {
	KmConfig kmconf;

	hostLabel = new QLabel(i18n("Remote hostname:"), this);
	hostEdit = new QLineEdit(this);
	hostEdit->setText(kmconf.getHostname());

	argsLabel = new QLabel(i18n("Nmap arguments:"), this);
	argsEdit = new QLineEdit(this);
	argsEdit->setText(kmconf.getArguments());

	setFixedHeight(75);

	connect(hostEdit, SIGNAL(returnPressed()), SLOT(slotHostReturn()) );
	connect(argsEdit, SIGNAL(returnPressed()), SLOT(slotArgsReturn()) );
}
Kmapview::~Kmapview(){
}

QString Kmapview::hostname()
{
	QString tmp = hostEdit->text();
	return tmp;
}
QString Kmapview::arguments()
{
	QString tmp = argsEdit->text();
	return tmp;
}

void Kmapview::slotStarted()
{
	hostEdit->setEnabled(false);
	argsEdit->setEnabled(false);
}
void Kmapview::slotExited()
{
	hostEdit->setEnabled(true);
	argsEdit->setEnabled(true);

	hostEdit->setFocus();
}
void Kmapview::slotReloadHostname()
{
	KmConfig kmconf;
	hostEdit->setText(kmconf.getHostname());
}

void Kmapview::slotReloadArguments()
{
	KmConfig kmconf;
	argsEdit->setText(kmconf.getArguments());
}

void Kmapview::slotHostReturn()
{
	// Transfer focus to args
	argsEdit->setFocus();
}
void Kmapview::slotArgsReturn()
{
	// Ask to start
	emit sigPleaseStart();
}

void Kmapview::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	hostLabel->setGeometry(10, 10, 140, 25);
	hostEdit->setGeometry(150, 10, width() - 165, 25);

	argsLabel->setGeometry(10, 40, 140, 25);
	argsEdit->setGeometry(150, 40, width() - 165, 25);
}

