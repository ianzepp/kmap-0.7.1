/***************************************************************************
                          kmoptother.cpp  -  description
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

#include "kmoptother.h"
#include "kmconfig.h"

KmOptOther::KmOptOther(QWidget *parent, const char *name) : QWidget(parent,name) {
	osidBox = new QCheckBox(i18n("OS Identification"), this, "CbOsId");
	identBox = new QCheckBox(i18n("Reverse ident scan"), this, "CbIdent");
	fastBox = new QCheckBox(i18n("Fast scan (services only)"), this, "CbFast");
	portBox = new QCheckBox(i18n("Specify a port range:"), this, "CbPort");
	fragBox = new QCheckBox(i18n("Fragment TCP headers"), this, "CbFrag");

	randBox = new QCheckBox(i18n("Randomize scans"), this);
	roffBox = new QCheckBox(i18n("Never use DNS"), this);
	ronBox = new QCheckBox(i18n("Always use DNS"), this);

	verboseBox = new QCheckBox(i18n("Enable verbosity"), this, "CbVerbose");
	debugBox = new QCheckBox(i18n("Enable debug"), this, "CbVerbose");

	verboseLvlBox = new QComboBox(this, "BoxVerbose");
	verboseLvlBox->insertItem(i18n("Level 1 verbose"));
	verboseLvlBox->insertItem(i18n("Level 2 verbose"));
	verboseLvlBox->insertItem(i18n("Level 3 verbose"));
	verboseLvlBox->insertItem(i18n("Level 4 verbose"));

	debugLvlBox = new QComboBox(this, "BoxDebug");
	debugLvlBox->insertItem(i18n("Level 1 debug"));
	debugLvlBox->insertItem(i18n("Level 2 debug"));
	debugLvlBox->insertItem(i18n("Level 3 debug (possible crash)"));
	debugLvlBox->insertItem(i18n("Level 4 debug (likely crash)"));

	portEdit = new QLineEdit(this, "LePort");

	connect(fastBox, SIGNAL(clicked()), SLOT(slotFastNotPort()));
	connect(portBox, SIGNAL(clicked()), SLOT(slotPortNotFast()));

	connect(roffBox, SIGNAL(clicked()), SLOT(slotRoffNotRon()));
	connect(ronBox, SIGNAL(clicked()), SLOT(slotRonNotRoff()));

	connect(portBox, SIGNAL(clicked()), SLOT(slotUpdate()));
	connect(verboseBox, SIGNAL(clicked()), SLOT(slotUpdate()));
	connect(debugBox, SIGNAL(clicked()), SLOT(slotUpdate()));

	LoadDefaults();
	Load();

	slotUpdate();
}
KmOptOther::~KmOptOther(){
}

QString KmOptOther::getArguments()
{
	KmConfig kmconf;
	QString tmp = "";
	QString port = portEdit->text();

	if (osidBox->isChecked())
		tmp += "-O ";

	if (identBox->isChecked())
		tmp += "-I ";

	if (fastBox->isChecked())
		tmp += "-F ";

	if (randBox->isChecked())
		tmp += "--randomize_hosts ";

	if (ronBox->isChecked())
		tmp += "-R ";

	if (roffBox->isChecked())
		tmp += "-n ";

	if ((portBox->isChecked()) && (!port.isEmpty()))
	{
		tmp += "-p ";
		tmp += port.stripWhiteSpace();
		tmp += " ";
	}

	if (fragBox->isChecked())
		tmp += "-f ";

	if (verboseBox->isChecked())
	{
		int lvl = verboseLvlBox->currentItem();

		if (lvl != -1)
		{
			for (int i = 0 ; i <= lvl ; i++)
				tmp += "-v ";
		}
	}

	if (debugBox->isChecked())
	{
		int lvl = debugLvlBox->currentItem();

		if (lvl != -1)
		{
			for (int i = 0 ; i <= lvl ; i++)
				tmp += "-d ";
		}
	}

	// Save port text
	kmconf.setPortRange(portEdit->text());

	return tmp;
}

void KmOptOther::Load()
{
	KmConfig kmconf;

	QString text = kmconf.getArguments();
	QString parm = "";
	QTextStream is(text, IO_ReadOnly);

	int i_verbose = -1;
	int i_debug = -1;

	// Load port text
	portEdit->setText(kmconf.getPortRange());

	// Loop through
	while (!is.eof())
	{
		is >> parm;

		check_parm: // only go here when errors
		if (parm == "-p")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parm;

			portBox->setChecked(true);
			portEdit->setText(parm);
		}

		if (parm == "-O")
			osidBox->setChecked(true);

		if (parm == "-I")
			identBox->setChecked(true);

		if (parm == "-F")
			fastBox->setChecked(true);

		if (parm == "-f")
			fragBox->setChecked(true);

		if (parm == "-v")
			i_verbose++;

		if (parm == "-d")
			i_debug++;

		if (parm == "-n")
		{
			roffBox->setChecked(true);
			slotRoffNotRon();
		}

		if (parm == "-R")
		{
			ronBox->setChecked(true);
			slotRonNotRoff();
		}

	}

	// Check verbose
	if (i_verbose > -1)
	{
		verboseBox->setChecked(true);

		if (i_verbose > 3)
			verboseLvlBox->setCurrentItem(3);
		else
			verboseLvlBox->setCurrentItem(i_verbose);
	}

	// Check debug
	if (i_debug > -1)
	{
		debugBox->setChecked(true);

		if (i_debug > 3)
			debugLvlBox->setCurrentItem(3);
		else
			debugLvlBox->setCurrentItem(i_verbose);
	}

	// Disabled frag if not root
	if (KmConfig::isRootUser() == false)
	{
		osidBox->setChecked(false);
		osidBox->setEnabled(false);

		fragBox->setChecked(false);
		fragBox->setEnabled(false);
	}

	slotUpdate();
}
void KmOptOther::LoadDefaults()
{
	// Clear paths
	portEdit->clear();
	
	osidBox->setChecked(false);
	identBox->setChecked(false);
	fastBox->setChecked(false);
	portBox->setChecked(false);
	fragBox->setChecked(false);

	randBox->setChecked(false);
	roffBox->setChecked(false);
	ronBox->setChecked(false);

	verboseBox->setChecked(false);
	verboseLvlBox->setCurrentItem(0);

	debugBox->setChecked(false);
	debugLvlBox->setCurrentItem(0);

	slotUpdate();
}

void KmOptOther::slotUpdate()
{
	portEdit->setEnabled(portBox->isChecked());
	verboseLvlBox->setEnabled(verboseBox->isChecked());
	debugLvlBox->setEnabled(debugBox->isChecked());
}

/*
 * Can only choose between fast or port
 */
void KmOptOther::slotFastNotPort()
{
	if (fastBox->isChecked())
		portBox->setChecked(false);
	slotUpdate();
}
/*
 * Can only choose between fast or port
 */
void KmOptOther::slotPortNotFast()
{
	if (portBox->isChecked())
		fastBox->setChecked(false);
	slotUpdate();
}

/*
 * Can only choose between resolv off or on or neither
 */
void KmOptOther::slotRoffNotRon()
{
	if (roffBox->isChecked())
		ronBox->setChecked(false);
	slotUpdate();
}
/*
 * Can only choose between resolv off or on or neither
 */
void KmOptOther::slotRonNotRoff()
{
	if (ronBox->isChecked())
		roffBox->setChecked(false);
	slotUpdate();
}

void KmOptOther::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	osidBox->setGeometry(10, 10, (width() / 2) - 10, 25);
	identBox->setGeometry(10, 40, (width() / 2) - 10, 25);
	fastBox->setGeometry(10, 70, (width() / 2) - 10, 25);
	fragBox->setGeometry(10, 100, (width() / 2) - 10, 25);
	randBox->setGeometry(10, 130, (width() / 2) - 10, 25);
	roffBox->setGeometry(10, 160, (width() / 2) - 10, 25);
	ronBox->setGeometry(10, 190, (width() / 2) - 10, 25);

	// Right side
	portBox->setGeometry((width() / 2), 10, (width() / 2) - 10, 25);
	portEdit->setGeometry((width() / 2), 40, (width() / 2) - 10, 25);

	verboseBox->setGeometry((width() / 2), 70, (width() / 2) - 10, 25);
	verboseLvlBox->setGeometry((width() / 2), 100, (width() / 2) - 10, 25);

	debugBox->setGeometry((width() / 2), 130, (width() / 2) - 10, 25);
	debugLvlBox->setGeometry((width() / 2), 160, (width() / 2) - 10, 25);




}


