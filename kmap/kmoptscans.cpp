/***************************************************************************
                          kmoptscans.cpp  -  description
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

#include "kmoptscans.h"
#include "kmconfig.h"

KmOptScans::KmOptScans(QWidget *parent, const char *name) : QWidget(parent,name) {
	// Create invis buttongroup
	scanGrp= new QButtonGroup(this, "BtnGroup");
	scanGrp->setExclusive(true);
	scanGrp->hide();

	// option widgets
	normalRadio = new QRadioButton(i18n("Normal-user scans:"), this, "BtnNormal");
	scanGrp->insert(normalRadio);

	rootRadio = new QRadioButton(i18n("Root-only scans:"), this, "BtnRoot");
	scanGrp->insert(rootRadio);

	bounceRadio = new QRadioButton(i18n("Ftp-server bounce:"), this, "BtnBouce");
	scanGrp->insert(bounceRadio);

	normalBox = new QComboBox(this, "BoxNormal");
	normalBox->insertItem(i18n("TCP connect() scan"));
	normalBox->insertItem(i18n("TCP ping scan"));
	normalBox->insertItem(i18n("TCP/UDP rcp scan"));

	rootBox = new QComboBox(this, "BoxRoot");
	rootBox->insertItem(i18n("TCP syn (half-open) scan"));
	rootBox->insertItem(i18n("TCP fin scan"));
	rootBox->insertItem(i18n("TCP Xmas tree scan"));
	rootBox->insertItem(i18n("TCP null packet scan"));
	rootBox->insertItem(i18n("UDP scan"));

	bounceEdit = new QLineEdit(this, "LeBounce");

	// Ping stuff
	pingLabel = new QLabel(this, "LblPing");
	pingLabel->setText(i18n("Available ping options:"));

	pingBox = new QComboBox(this, "BoxPing");
	pingBox->insertItem(i18n("Don't ping the target host"));
	pingBox->insertItem(i18n("Use TCP ping instead of ICMP"));
	pingBox->insertItem(i18n("Use ICMP ping instead of TCP"));
	pingBox->insertItem(i18n("Use TCP & ICMP in parallel"));

	portBox = new QCheckBox(this, "BoxPort");
	portBox->setText(i18n("Set TCP ping to port:"));

	portEdit = new QLineEdit(this, "LePortText");

	synBox = new QCheckBox(this, "BoxSyn");
	synBox->setText(i18n("Use SYN instead of ACK for TCP packets"));

	// end of widgets

	connect(normalRadio, SIGNAL(clicked()), SLOT(slotUpdate()));
	connect(rootRadio, SIGNAL(clicked()), SLOT(slotUpdate()));
	connect(bounceRadio, SIGNAL(clicked()), SLOT(slotUpdate()));

	connect(pingBox, SIGNAL(activated(int)), SLOT(slotUpdate()));
	connect(portBox, SIGNAL(clicked()), SLOT(slotUpdate()));

	LoadDefaults();
	Load();

	slotUpdate();
}
KmOptScans::~KmOptScans(){
	KmConfig kmconf;
	kmconf.setFtpBounceHost(bounceEdit->text());
	kmconf.setUsePingPort(portEdit->text());
}

QString KmOptScans::getArguments()
{
	KmConfig kmconf;
	QString tmp = "";

	if (normalRadio->isChecked())
	{
		if (normalBox->currentItem() == 0)
			tmp += "-sT ";
		else if (normalBox->currentItem() == 1)
			tmp += "-sP ";
		else if (normalBox->currentItem() == 2)
			tmp += "-sR ";
	}
	else if (rootRadio->isChecked())
	{
		if (normalBox->currentItem() == 0)
			tmp += "-sS ";
		else if (normalBox->currentItem() == 1)
			tmp += "-sF ";
		else if (normalBox->currentItem() == 2)
			tmp += "-sX ";
		else if (normalBox->currentItem() == 3)
			tmp += "-sN ";
		else if (normalBox->currentItem() == 4)
			tmp += "-sU ";
	}
	else if ((bounceRadio->isChecked()) && (bounceEdit->text() != ""))
	{
		tmp += "-b ";
		tmp += bounceEdit->text();
		tmp += " ";
	}

	// Always save ftp host
	kmconf.setFtpBounceHost(bounceEdit->text());

	// Setup pings
	if (pingBox->currentItem() == 0) // no ping
	{
		tmp += "-P0 ";
	}
	else if (pingBox->currentItem() == 1) // tcp ping
	{
		// Special handling for this one
		if (portBox->isChecked())
		{
			tmp += "-PT";
			tmp += portEdit->text();
			tmp += " ";
		}

		if (synBox->isChecked())
			tmp += "-PS ";
	}
	else if (pingBox->currentItem() == 2) // icmp ping
	{
		tmp += "-PI ";
	}
	else if (pingBox->currentItem() == 3) // both icmp & tcp ping
	{
		// Special handling for this one too
		if (portBox->isChecked())
		{
			tmp += "-PB";
			tmp += portEdit->text();
			tmp += " ";
		}

		if (synBox->isChecked())
			tmp += "-PS ";
	}

	return tmp;
}

void KmOptScans::Load()
{
	KmConfig kmconf;

	QString text = kmconf.getArguments();
	QString parm = "";
	QTextStream is(text, IO_ReadOnly);

	// Set last bounce host
	bounceEdit->setText(kmconf.getFtpBounceHost());

	// Also load old port setting even if it is not used.
	portEdit->setText(kmconf.getUsePingPort());

	// Load over with real options
	while (!is.eof())
	{
		is >> parm;

		// Normal scans
		check_parm: // only used if errors

		if (parm == "-sT")
			slotNormal(0);
		if (parm == "-sP")
			slotNormal(1);
		if (parm == "-sR")
			slotNormal(2);

		// Root scans
		if (parm == "-sS")
			slotRoot(0);
		if (parm == "-sF")
			slotRoot(1);
		if (parm == "-sX")
			slotRoot(2);
		if (parm == "-sN")
			slotRoot(3);
		if (parm == "-sU")
			slotRoot(4);

		// Ftp bounce
		if (parm == "-b")
		{
			is >> parm; // get host

			if (parm.left(1) == "1")
				goto check_parm; // bad bounce option

			bounceEdit->setText(parm);
			slotBounce();
		}

		//
		// Pings
		//
		if (parm == "-P0")
			pingBox->setCurrentItem(0); // no ping

		if (parm.left(3) == "-PT")
		{
			pingBox->setCurrentItem(1); // no ping

			// Check for a specific port
			parm = parm.remove(0, 3);

			if (!parm.isEmpty())
			{
				portBox->setChecked(true);
				portEdit->setText(parm);
			}
		}

		if (parm == "-PI")
			pingBox->setCurrentItem(2); // icmp

		if (parm.left(3) == "-PB")
		{
			pingBox->setCurrentItem(1); // icmp & tcp

			// Check for a specific port
			parm = parm.remove(0, 3);

			if (!parm.isEmpty())
			{
				portBox->setChecked(true);
				portEdit->setText(parm);
			}
		}

		// Check for syn targeting
		if (parm == "-PS")
		{
			synBox->setChecked(true);
		}
	}

	// Close down certain selections
	if (KmConfig::isRootUser() == false)
	{
		rootRadio->setEnabled(false);
		rootBox->setEnabled(false);

		// Was it checked? If so switch
		if (rootRadio->isChecked())
			slotNormal(0);
	}
	
}
void KmOptScans::LoadDefaults()
{
	slotNormal(0);
	bounceEdit->clear();

	pingBox->setCurrentItem(3); //icmp & tcp
	portBox->setChecked(false);
	portEdit->setText("");
	synBox->setChecked(false);
}

void KmOptScans::slotUpdate()
{
	normalBox->setEnabled(normalRadio->isChecked());
	rootBox->setEnabled(rootRadio->isChecked());
	bounceEdit->setEnabled(bounceRadio->isChecked());

	switch (pingBox->currentItem())
	{
		case 1: // tcp
		case 3: // icmp and tcp
			portBox->setEnabled(true);
			portEdit->setEnabled(portBox->isChecked());
			synBox->setEnabled(true);
		break;

		default:
			portBox->setEnabled(false);
			portEdit->setEnabled(false);
			synBox->setEnabled(false);
		break;
	}
}

void KmOptScans::slotNormal(int index)
{
	normalBox->setCurrentItem(index);

	normalRadio->setChecked(true);
	rootRadio->setChecked(false);
	bounceRadio->setChecked(false);

	slotUpdate();
}
void KmOptScans::slotRoot(int index)
{
	rootBox->setCurrentItem(index);

	normalRadio->setChecked(false);
	rootRadio->setChecked(true);
	bounceRadio->setChecked(false);

	slotUpdate();
}
void KmOptScans::slotBounce()
{
	normalRadio->setChecked(false);
	rootRadio->setChecked(false);
	bounceRadio->setChecked(true);

	slotUpdate();
}

void KmOptScans::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	normalRadio->setGeometry(10, 10, 150, 25);
	rootRadio->setGeometry(10, 40, 150, 25);
	bounceRadio->setGeometry(10, 70, 150, 25);

	normalBox->setGeometry(170, 10, width() - 180, 25);
	rootBox->setGeometry(170, 40, width() - 180, 25);
	bounceEdit->setGeometry(170, 70, width() - 180, 25);

	pingLabel->setGeometry( 10 , 120 , 150 , 25 );
	pingBox->setGeometry( 170 , 120 , width() - 180 , 25 );

	portBox->setGeometry( 20 , 150 , width() - 100 , 25 );
	portEdit->setGeometry( width() - 90 , 150 , 80 , 25 );

	synBox->setGeometry( 20 , 180 , width() - 40 , 25 );
}






