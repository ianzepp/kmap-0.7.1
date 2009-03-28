/***************************************************************************
                          kmoptpaths.cpp  -  description
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

#include "kmoptpaths.h"
#include "kmconfig.h"

KmOptPaths::KmOptPaths(QWidget *parent, const char *name) : QWidget(parent,name) {
	nmapLabel = new QLabel(i18n("Nmap full path:"), this);
	nmapEdit = new QLineEdit(this);
	nmapBtn = new QPushButton(i18n("Browse"), this);

	hlogBox = new QCheckBox(i18n("Log in human format"), this, "CbHlog");
	hlogEdit = new QLineEdit(this, "LeHlog");
	hlogBtn = new QPushButton(i18n("Browse"), this);
	
	mlogBox = new QCheckBox(i18n("Log in machine format"), this, "CbMlog");
	mlogEdit = new QLineEdit(this, "LeMlog");
	mlogBtn = new QPushButton(i18n("Browse"), this);

	connect(hlogBox, SIGNAL(clicked()), SLOT(slotUpdate()));
	connect(mlogBox, SIGNAL(clicked()), SLOT(slotUpdate()));

	connect(nmapBtn, SIGNAL(clicked()), SLOT(slotNmapBtn()));
	connect(hlogBtn, SIGNAL(clicked()), SLOT(slotHlogBtn()));
	connect(mlogBtn, SIGNAL(clicked()), SLOT(slotMlogBtn()));

	LoadDefaults();
	Load();

	slotUpdate();
}
KmOptPaths::~KmOptPaths(){
}

QString KmOptPaths::getArguments()
{
	QString tmp = "";

	QString hpath = hlogEdit->text();
	QString mpath = mlogEdit->text();

	if ((hlogBox->isChecked()) && (!hpath.isEmpty()))
	{
		tmp += "-o ";
		tmp += hpath.stripWhiteSpace();
		tmp += " ";
	}

	if ((mlogBox->isChecked()) && (!mpath.isEmpty()))
	{
		tmp += "-m ";
		tmp += mpath.stripWhiteSpace();
		tmp += " ";
	}

	// Now save the strings for future use
	KmConfig kmconf;
	QString text;

	// Save nmap path if there
	text = nmapEdit->text();
	if (!text.isEmpty())
		kmconf.setNmapExecutable(text);

	// Save hlog path if there regardless
	text = hlogEdit->text();
	if (!text.isEmpty())
		kmconf.setHumanLogPath(text);

	// Save mlog path if there regardless
	text = mlogEdit->text();
	if (!text.isEmpty())
		kmconf.setMachineLogPath(text);

	return tmp;
}

void KmOptPaths::Load()
{
	KmConfig kmconf;

	QString text = kmconf.getArguments();
	QString parm = "";
	QTextStream is(text, IO_ReadOnly);

	while (!is.eof())
	{
		is >> parm;

		check_parms:
		if (parm == "-o")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;

			hlogBox->setChecked(true);
			hlogEdit->setText(parm);
		}

		if (parm == "-m")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;

			mlogBox->setChecked(true);
			mlogEdit->setText(parm);
		}
	}

	// Set nmap path if there:
	nmapEdit->setText(kmconf.getNmapExecutable());

	if (strlen(hlogEdit->text()) == 0)
		hlogEdit->setText(kmconf.getHumanLogPath());

	if (strlen(mlogEdit->text()) == 0)
		mlogEdit->setText(kmconf.getMachineLogPath());

	slotUpdate();
}

void KmOptPaths::LoadDefaults()
{
	// Clear paths
	hlogEdit->clear();
	mlogEdit->clear();
	
	hlogBox->setChecked(false);
	mlogBox->setChecked(false);

	slotUpdate();
}

void KmOptPaths::slotUpdate()
{
	hlogEdit->setEnabled(hlogBox->isChecked());
	hlogBtn->setEnabled(hlogBox->isChecked());

	mlogEdit->setEnabled(mlogBox->isChecked());
	mlogBtn->setEnabled(mlogBox->isChecked());
}

void KmOptPaths::slotNmapBtn()
{
	QString text = QFileDialog::getOpenFileName("/usr/bin");
	
	if (text.isEmpty())
		return;

	nmapEdit->setText(text);
}
void KmOptPaths::slotHlogBtn()
{
	// allows to choose a new name
	QString text = QFileDialog::getSaveFileName(QDir::homeDirPath());
	
	if (text.isEmpty())
		return;

	hlogEdit->setText(text);
}
void KmOptPaths::slotMlogBtn()
{
	// allows to choose a new name
	QString text = QFileDialog::getSaveFileName(QDir::homeDirPath());
	
	if (text.isEmpty())
		return;

	mlogEdit->setText(text);
}

void KmOptPaths::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	nmapLabel->setGeometry(10, 10, width() - 20, 25);
	nmapEdit->setGeometry(10, 40, width() - 120, 25);
	nmapBtn->setGeometry(width() - 100, 40, 90, 25);

	hlogBox->setGeometry(10, 70, width() - 20, 25);
	hlogEdit->setGeometry(10, 100, width() - 120, 25);
	hlogBtn->setGeometry(width() - 100, 100, 90, 25);

	mlogBox->setGeometry(10, 130, width() - 20, 25);
	mlogEdit->setGeometry(10, 160, width() - 120, 25);
	mlogBtn->setGeometry(width() - 100, 160, 90, 25);

}



