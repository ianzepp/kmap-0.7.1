/***************************************************************************
                          kmhistory.cpp  -  description
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

#include "kmhistory.h"
#include "kmconfig.h"

KmHistory::KmHistory(QWidget *parent, const char *name ) : QTabDialog(parent,name,true) {
	setOkButton();
	setCancelButton();

	kmhist = new KmHistoryWidget(this);

	connect(kmhist, SIGNAL(sigReloadHostname()),
		SIGNAL(sigReloadHostname()) );
	connect(kmhist, SIGNAL(sigReloadArguments()),
		SIGNAL(sigReloadArguments()) );
	connect(kmhist, SIGNAL(sigAccept()),
		SLOT(accept()) );

	connect(this, SIGNAL(applyButtonPressed()), SLOT(slotApply()) );
	connect(this, SIGNAL(cancelButtonPressed()), SLOT(slotCancel()) );

	this->addTab(kmhist, i18n("History List"));

	LoadDefaults();
	Load();
}
KmHistory::~KmHistory(){
}

void KmHistory::Load()
{
	kmhist->Load();
}
void KmHistory::LoadDefaults()
{
	kmhist->LoadDefaults();
}

/*
 * Saves to settings
 */
void KmHistory::slotApply()
{
	kmhist->Save();
}
void KmHistory::slotCancel()
{
}

/*
 *
 *
 *
 *
 *
 *
 */

KmHistoryWidget::KmHistoryWidget(QWidget *parent, const char *name ) : QWidget(parent,name) {
	histList = new QListView(this);
	histList->setAllColumnsShowFocus(true);
	histList->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

	histList->addColumn(i18n("Host"));
	histList->addColumn(i18n("Options"));

	removeBtn = new QPushButton(i18n("Remove"), this);
	clearBtn = new QPushButton(i18n("Clear"), this);

	connect(histList, SIGNAL(selectionChanged(QListViewItem *)),
		SLOT(slotSelection(QListViewItem *)) );
	connect(histList, SIGNAL(doubleClicked(QListViewItem *)),
		SLOT(slotDoubleClicked(QListViewItem *)) );

	connect(removeBtn, SIGNAL(clicked()), SLOT(slotRemoveBtn()) );
	connect(clearBtn, SIGNAL(clicked()), SLOT(slotClearBtn()) );

	LoadDefaults();
	Load();
}
KmHistoryWidget::~KmHistoryWidget(){
}

void KmHistoryWidget::slotSelection(QListViewItem *item)
{

}

void KmHistoryWidget::slotDoubleClicked(QListViewItem *item)
{
	KmConfig kmconf;
	kmconf.setHostname(item->text(0));
	kmconf.setArguments(item->text(1));

	emit sigReloadHostname();
	emit sigReloadArguments();
	emit sigAccept();
}

void KmHistoryWidget::Load()
{
	KmConfig kmconf;
	QStrList list = kmconf.getHistoryList();

        QString text;
	QString host;
	QString args;

	int pos;
	
	for (uint i = 0; i < list.count(); i++)
	{
		text = list.at(i);
		pos  = text.find(" ");

		if (pos != -1)
		{
			host = text.left(pos);
			args = text.remove(0, pos + 1);

			(void) new QListViewItem(histList, host, args);
		}
	}
}
void KmHistoryWidget::LoadDefaults()
{
	histList->clear();
}
void KmHistoryWidget::Save()
{
	QString host = "";
	QString args = "";
	QString line = "";
	QStrList list;
	QListViewItem *item = histList->firstChild();

	// Run through list
	while (item != 0)
	{
		host = item->text(0);
		args = item->text(1);

		line = host + " " + args;
		list.append(line);

		item = item->nextSibling();
	}

	// Save list
	KmConfig::setHistoryList(list);
}

void KmHistoryWidget::slotRemoveBtn()
{
	if (histList->currentItem() == 0)
		return;
	histList->currentItem()->~QListViewItem();
}
void KmHistoryWidget::slotClearBtn()
{
	histList->clear();
}

void KmHistoryWidget::paintEvent( QPaintEvent *event )
{
	this->QWidget::paintEvent(event);

	histList->setGeometry(10, 10, width() - 120, height() - 20);
	removeBtn->setGeometry(width() - 100, 10, 90, 25);
	clearBtn->setGeometry(width() - 100, 40, 90, 25);

	int w_avg = (histList->width() - 20) / 3;

	histList->setColumnWidth(0, w_avg);
	histList->setColumnWidth(1, w_avg * 2);
}
