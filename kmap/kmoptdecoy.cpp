/***************************************************************************
                          kmoptdecoy.cpp  -  description
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

#include "kmoptdecoy.h"
#include "kmconfig.h"

KmOptDecoy::KmOptDecoy(QWidget *parent, const char *name) : QWidget(parent,name) {
	enableCheck = new QCheckBox(i18n("Enable decoy use"), this);

	listBox = new QListBox(this, "LbList");
	listBox->setFrameStyle(QFrame::Sunken | QFrame::WinPanel);

	editBox = new QLineEdit(this);
	
	addBtn = new QPushButton(i18n("Add"), this);
	delBtn = new QPushButton(i18n("Remove"), this);
	upBtn = new QPushButton(i18n("Shift Up"), this);
	downBtn = new QPushButton(i18n("Shift Down"), this);

	connect(enableCheck, SIGNAL(clicked()), SLOT(slotUpdate()) );

	connect(addBtn, SIGNAL(clicked()), SLOT(slotAdd()) );
	connect(delBtn, SIGNAL(clicked()), SLOT(slotDel()) );
	connect(upBtn, SIGNAL(clicked()), SLOT(slotUp()) );
	connect(downBtn, SIGNAL(clicked()), SLOT(slotDown()) );

	connect(listBox, SIGNAL(highlighted(int)),SLOT(slotHighlighted(int)));
	connect(listBox, SIGNAL(selected(int)), SLOT(slotSelected(int)));

	LoadDefaults();
	Load();

	slotUpdate();
}
KmOptDecoy::~KmOptDecoy(){
}

QString KmOptDecoy::getArguments()
{
	QString tmp = "";

	if (listBox->count() == 0)
		return tmp;

	if (enableCheck->isChecked() == false)
		return tmp;

	// Start
	tmp += "-D ";

	// Get all but last one
	for (uint i = 0 ; i < listBox->count() - 1; i++)
	{
		tmp += listBox->text(i);
		tmp += ",";
	}

	// Add last decoy, has to be at least one here
	tmp += listBox->text(listBox->count() - 1);
	tmp += " ";

	return tmp;
}

void KmOptDecoy::Load()
{
	KmConfig kmconf;

	// Are we even allowed decoys?
	if (KmConfig::isRootUser() == false)
	{
		enableCheck->setEnabled(false);
		enableCheck->setChecked(false);
		enableCheck->setText(i18n("Enable decoy use (must be root)"));
		return;
	}

	QString text = kmconf.getArguments();
	QString parm = "";
	QTextStream is(text, IO_ReadOnly);

	while (!is.eof())
	{
		is >> parm;

		if (parm == "-D")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				continue; // abort

			// Parm should be comma delimated
			int pos = -1;

			while ((pos = parm.find(",")) != -1)
			{
				listBox->insertItem(parm.left(pos), -1);
				parm = parm.remove(0, pos+1);
			}

			// Anything left?.
			if (!parm.isEmpty())
				listBox->insertItem(parm, -1);

			// Finally, enable
			enableCheck->setChecked(true);
		}
	}
}
void KmOptDecoy::LoadDefaults()
{
	editBox->clear();
	listBox->clear();
	enableCheck->setChecked(false);
}

void KmOptDecoy::slotUpdate()
{
	bool enable = enableCheck->isChecked();

	editBox->setEnabled(enable);
	listBox->setEnabled(enable);
	addBtn->setEnabled(enable);
	delBtn->setEnabled(enable);
	upBtn->setEnabled(enable);
	downBtn->setEnabled(enable);
}

void KmOptDecoy::slotHighlighted(int i)
{
}
void KmOptDecoy::slotSelected(int i)
{
}

void KmOptDecoy::slotAdd()
{
	QString tmp = editBox->text();

	if (tmp.isEmpty())
		return;

	listBox->insertItem(tmp, -1);
	editBox->clear();
}
void KmOptDecoy::slotDel()
{
	int pos = listBox->currentItem();

	if (pos == -1)
		return;

	listBox->removeItem(pos);
}
void KmOptDecoy::slotUp()
{
	int pos = listBox->currentItem();

	if (pos < 1)
		return; // has to be at least second item

	QString text = listBox->text(pos);

	listBox->removeItem(pos);
	listBox->insertItem(text, --pos);

	listBox->setCurrentItem(pos);
}
void KmOptDecoy::slotDown()
{
	int pos = listBox->currentItem();

	if (pos == -1)
		return;

	if (pos >= (int) listBox->count() - 1)
		return; // has to be at least second from last item

	QString text = listBox->text(pos);

	listBox->removeItem(pos);
	listBox->insertItem(text, ++pos); // insert right back in

	listBox->setCurrentItem(pos);
}

void KmOptDecoy::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	enableCheck->setGeometry(10, 10, width() - 120, 25);

	editBox->setGeometry(10, 40, width() - 120, 25);
	addBtn->setGeometry(width() - 100, 40, 90, 25);

	listBox->setGeometry(10, 70, width() - 120, height() - 80);

	upBtn->setGeometry(width() - 100, 70, 90, 25);
	downBtn->setGeometry(width() - 100, 100, 90, 25);
	delBtn->setGeometry(width() - 100, 130, 90, 25);
}



