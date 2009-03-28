/***************************************************************************
                          kmlogdlg.cpp  -  description
                             -------------------
    begin                : Sun Jan 9 2000
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

#include "kmlogdlg.h"
#include "kmconfig.h"

KmLogDlg::KmLogDlg(QWidget *parent, const char *name ) : QWidget(parent,name) {
	// Port Widgets
	savePortBox = new QCheckBox(i18n("Save to log:"), this);
	savePortBox->setEnabled(true);

	pathPortEdit = new QLineEdit(this);
	pathPortEdit->setEnabled(false);

	browsePortBtn = new QPushButton(i18n("Browse"), this);
	browsePortBtn->setEnabled(false);

	savePortBtn = new QPushButton(i18n("Save"), this);
	savePortBtn->setEnabled(false);

	// Info Widgets
	saveInfoBox = new QCheckBox(i18n("Save to log:"), this);
	saveInfoBox->setEnabled(true);

	pathInfoEdit = new QLineEdit(this);
	pathInfoEdit->setEnabled(false);

	browseInfoBtn = new QPushButton(i18n("Browse"), this);
	browseInfoBtn->setEnabled(false);

	saveInfoBtn = new QPushButton(i18n("Save"), this);
	saveInfoBtn->setEnabled(false);

	// Nmap Widgets
	saveNmapBox = new QCheckBox(i18n("Save to log:"), this);
	saveNmapBox->setEnabled(true);

	pathNmapEdit = new QLineEdit(this);
	pathNmapEdit->setEnabled(false);

	browseNmapBtn = new QPushButton(i18n("Browse"), this);
	browseNmapBtn->setEnabled(false);

	saveNmapBtn = new QPushButton(i18n("Save"), this);
	saveNmapBtn->setEnabled(false);

	// sigs and slots
	connect(savePortBox, SIGNAL(clicked()), SLOT(slotPortLogFile()));
	connect(pathPortEdit, SIGNAL(returnPressed()), SLOT(slotPortBtnSave()));
	connect(browsePortBtn, SIGNAL(clicked()), SLOT(slotPortBtnBrowse()));
	connect(savePortBtn, SIGNAL(clicked()), SLOT(slotPortBtnSave()));

	connect(saveInfoBox, SIGNAL(clicked()), SLOT(slotInfoLogFile()));
	connect(pathInfoEdit, SIGNAL(returnPressed()), SLOT(slotInfoBtnSave()));
	connect(browseInfoBtn, SIGNAL(clicked()), SLOT(slotInfoBtnBrowse()));
	connect(saveInfoBtn, SIGNAL(clicked()), SLOT(slotInfoBtnSave()));

	connect(saveNmapBox, SIGNAL(clicked()), SLOT(slotNmapLogFile()));
	connect(pathNmapEdit, SIGNAL(returnPressed()), SLOT(slotNmapBtnSave()));
	connect(browseNmapBtn, SIGNAL(clicked()), SLOT(slotNmapBtnBrowse()));
	connect(saveNmapBtn, SIGNAL(clicked()), SLOT(slotNmapBtnSave()));

	// Load old paths
	KmConfig kmconf;
	pathPortEdit->setText(kmconf.getPortViewLog());
	pathInfoEdit->setText(kmconf.getInfoViewLog());
	pathNmapEdit->setText(kmconf.getNmapViewLog());
	
}
KmLogDlg::~KmLogDlg(){
}

//
// Port functions
//
void KmLogDlg::slotPortLogFile()
{
	pathPortEdit->setEnabled(savePortBox->isChecked());
	browsePortBtn->setEnabled(savePortBox->isChecked());
	savePortBtn->setEnabled(savePortBox->isChecked());
}
void KmLogDlg::slotPortBtnBrowse()
{
	QString text = QFileDialog::getSaveFileName();

	if (text.isEmpty())
		return;

	pathPortEdit->setText(text);

	KmConfig kmconf;
	kmconf.setPortViewLog(text);
}
void KmLogDlg::slotPortBtnSave()
{
	QString filename = pathPortEdit->text();

	if (filename.isEmpty()) {
		QMessageBox::information(0, i18n("Error: No filename given"),
		 	  i18n("No filename was given. Please choose a filename first."));
		return;
	}

	emit sigSavePort();
}

//
// Info functions
//
void KmLogDlg::slotInfoLogFile()
{
	pathInfoEdit->setEnabled(saveInfoBox->isChecked());
	browseInfoBtn->setEnabled(saveInfoBox->isChecked());
	saveInfoBtn->setEnabled(saveInfoBox->isChecked());
}
void KmLogDlg::slotInfoBtnBrowse()
{
	QString text = QFileDialog::getSaveFileName();

	if (text.isEmpty())
		return;

	pathInfoEdit->setText(text);

	KmConfig kmconf;
	kmconf.setInfoViewLog(text);
}
void KmLogDlg::slotInfoBtnSave()
{
	QString filename = pathInfoEdit->text();

	if (filename.isEmpty()) {
		QMessageBox::information(0, i18n("Error: No filename given"),
		 	  i18n("No filename was given. Please choose a filename first."));
		return;
	}

	emit sigSaveInfo();
}

//
// Nmap functions
//
void KmLogDlg::slotNmapLogFile()
{
	pathNmapEdit->setEnabled(saveNmapBox->isChecked());
	browseNmapBtn->setEnabled(saveNmapBox->isChecked());
	saveNmapBtn->setEnabled(saveNmapBox->isChecked());
}
void KmLogDlg::slotNmapBtnBrowse()
{
	QString text = QFileDialog::getSaveFileName();

	if (text.isEmpty())
		return;

	pathNmapEdit->setText(text);

	KmConfig kmconf;
	kmconf.setNmapViewLog(text);
}
void KmLogDlg::slotNmapBtnSave()
{
	QString filename = pathNmapEdit->text();

	if (filename.isEmpty()) {
		QMessageBox::information(0, i18n("Error: No filename given"),
		 	  i18n("No filename was given. Please choose a filename first."));
		return;
	}

	emit sigSaveNmap();
}
void KmLogDlg::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	savePortBox->setGeometry(10, 10, width() - 20, 25);
	pathPortEdit->setGeometry(10, 40, width() - 220, 25);
	browsePortBtn->setGeometry(width() - 200, 40, 90, 25);
	savePortBtn->setGeometry(width() - 100, 40, 90, 25);

	saveInfoBox->setGeometry(10, 70, width() - 20, 25);
	pathInfoEdit->setGeometry(10, 100, width() - 220, 25);
	browseInfoBtn->setGeometry(width() - 200, 100, 90, 25);
	saveInfoBtn->setGeometry(width() - 100, 100, 90, 25);

	saveNmapBox->setGeometry(10, 130, width() - 20, 25);
	pathNmapEdit->setGeometry(10, 170, width() - 220, 25);
	browseNmapBtn->setGeometry(width() - 200, 170, 90, 25);
	saveNmapBtn->setGeometry(width() - 100, 170, 90, 25);

}


