/***************************************************************************
                          kmnmapview.cpp  -  description
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

#include "kmnmapview.h"
#include "kmconfig.h"

KmNmapView::KmNmapView(QWidget *parent, const char *name) : QWidget(parent,name) {
	this->setCaption(i18n("Kmap: Nmap Output"));

	textView = new QMultiLineEdit(this, "MleView");
	textView->setReadOnly(true);

	saveBox = new QCheckBox(i18n("Save to log:"), this, "CbSaveLog");
	saveBox->setEnabled(true);

	pathEdit = new QLineEdit(this, "LeLogFile");
	pathEdit->setEnabled(false);

	browseBtn = new QPushButton(i18n("Browse"), this, "BtnBrowse");
	browseBtn->setEnabled(false);

	saveBtn = new QPushButton(i18n("Save"), this, "BtnSave");
	saveBtn->setEnabled(false);

	connect(saveBox, SIGNAL(clicked()), SLOT(slotLogFile()));
	connect(pathEdit, SIGNAL(returnPressed()), SLOT(slotBtnSave()));
	connect(browseBtn, SIGNAL(clicked()), SLOT(slotBtnBrowse()));
	connect(saveBtn, SIGNAL(clicked()), SLOT(slotBtnSave()));

	// Load old path
	KmConfig kmconf;
	pathEdit->setText(kmconf.getNmapViewLog());
}
KmNmapView::~KmNmapView(){
}

void KmNmapView::Clear()
{
	textView->clear();
}


void KmNmapView::Append(const QString &text)
{
	textView->append(text);
	textView->setCursorPosition(textView->numLines(), 1);
}

void KmNmapView::slotStarted()
{
	Clear();
}

void KmNmapView::slotStdout(const QString &text)
{
	Append(text);
}
void KmNmapView::slotStderr(const QString &text)
{
	Append(text);
}
void KmNmapView::slotExited()
{
}

void KmNmapView::slotLogFile()
{
	pathEdit->setEnabled(saveBox->isChecked());
	browseBtn->setEnabled(saveBox->isChecked());
	saveBtn->setEnabled(saveBox->isChecked());
}
void KmNmapView::slotBtnBrowse()
{
	QString text = QFileDialog::getSaveFileName();

	if (text.isEmpty())
		return;

	pathEdit->setText(text);

	KmConfig kmconf;
	kmconf.setNmapViewLog(text);
}
void KmNmapView::slotBtnSave()
{
	QString filename = pathEdit->text();

	if (filename.isEmpty()) {
		QMessageBox::information(0, i18n("Error: No filename given"),
		 	  i18n("No filename was given. Please choose a filename first."));
		return;
	}

	QString text = textView->text();

	if (text.isEmpty()) {
		QMessageBox::information(0, i18n("Error: No text to save"),
			i18n("No text in the output window to save"));
		return;
	}

	// Prepend data and stuff
	text.prepend("Generated at: " + QDate::currentDate().toString() + "\n");
	text.append("\n");

	// Open file
	QFile f(filename);

	if (f.open(IO_WriteOnly) == false) {
		QMessageBox::warning(0, i18n("Error: Unable to save"),
			i18n("Unable to save: write permission denied"));
		return;
	}

	// Save data
	if (f.writeBlock(text, text.length()) == -1) {
		QMessageBox::warning(0, i18n("Error: Write error"),
			i18n("Write error: unknown error"));
		return;
	}

	// Flush
	f.flush();

	// Done.
	QMessageBox::information(0, i18n("File saved"),
		i18n("Successfully saved output text"));
}

void KmNmapView::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	textView->setGeometry(0, 0, width(), height() - 35);

	saveBox->setGeometry(5, height() - 30, 90, 25);
	pathEdit->setGeometry(100, height() - 30, width() - 300, 25);
	browseBtn->setGeometry(width() - 195, height() - 30, 90, 25);
	saveBtn->setGeometry(width() - 95, height() - 30, 90, 25);
}

/*
 * Asks the main kmap to hide me nicely
 */
void KmNmapView::closeEvent( QCloseEvent *event )
{
	QWidget::closeEvent(event);

	// Tell hidden
	emit sigHidden();
}






