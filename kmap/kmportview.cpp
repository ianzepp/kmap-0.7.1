/***************************************************************************
                          kmportview.cpp  -  description
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

#include "kmportview.h"
#include "kmconfig.h"

KmPortView::KmPortView(QWidget *parent, const char *name ) : QWidget(parent,name) {
	this->setCaption(i18n("Kmap: Port Results"));

	mainView = new QListView(this);
	mainView->setAllColumnsShowFocus(true);
	mainView->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

	mainView->addColumn(i18n("Address"));
	mainView->addColumn(i18n("Port"));
	mainView->addColumn(i18n("State"));
	mainView->addColumn(i18n("Protocol"));
	mainView->addColumn(i18n("Service"));

	saveBox = new QCheckBox(i18n("Save to log:"), this);
	saveBox->setEnabled(true);

	pathEdit = new QLineEdit(this);
	pathEdit->setEnabled(false);

	browseBtn = new QPushButton(i18n("Browse"), this);
	browseBtn->setEnabled(false);

	saveBtn = new QPushButton(i18n("Save"), this);
	saveBtn->setEnabled(false);

	connect(saveBox, SIGNAL(clicked()), SLOT(slotLogFile()));
	connect(pathEdit, SIGNAL(returnPressed()), SLOT(slotBtnSave()));
	connect(browseBtn, SIGNAL(clicked()), SLOT(slotBtnBrowse()));
	connect(saveBtn, SIGNAL(clicked()), SLOT(slotBtnSave()));

	// Load old path
	KmConfig kmconf;
	pathEdit->setText(kmconf.getPortViewLog());

	in_port_list = false;
	is_ping_scan = false;
	current_ip = "";
}
KmPortView::~KmPortView(){
}

/*
 * Clears the window
 */
void KmPortView::Clear()
{
	mainView->clear();
}

/*
 * Handles the signal that nmap has just started
 */
void KmPortView::slotStarted()
{
	in_port_list = false;
	is_ping_scan = false;
	current_ip = "";

	Clear();

	// Check for a ping scan
	KmConfig kmconf;
	QString tmp = kmconf.getArguments();

	if (tmp.find("-sP") != -1)
		is_ping_scan = true;
}

/*
 * Handles stdout text from nmap
 */
void KmPortView::slotStdout( const QString &text )
{
	int pos = -1;

	if (in_port_list)
		handlePortEntry(text);

	// Grab the IP address?
	pos = text.find(QRegExp("^Interesting ports on "));

	if (pos != -1)
		handleIpAddress( text );

	// Grab the ping state?
	pos = text.find(QRegExp("^Host [a-zA-Z\\s]"));

	if ((is_ping_scan) && (pos != -1))
		handlePingState(text, true);

	// Are we entering the port listing?
	pos = text.find(QRegExp("^Port "));

	if ((!in_port_list) && (pos != -1 ))
		in_port_list = true;
}

/*
 * Handles stderr text from nmap
 */
void KmPortView::slotStderr( const QString &text )
{
	// normal messages don't come on stderr
}

/*
 * Triggered when nmap has exited
 */
void KmPortView::slotExited()
{
	// No resets neccessary yet
}

/*
 * Receives a line of input from nmap and saves the current
 * ip address being scanned.
 */
void KmPortView::handleIpAddress( const QString &text )
{
	int start = text.find("(") + 1;
	int stop = text.find(")");
	int len = stop - start;

	current_ip = text.mid(start, len);
}

/*
 * Adds an entry to the window ONLY if the target is up or active
 */
void KmPortView::handlePingState( const QString &text , bool state )
{
	int start = text.find("(") + 1;
	int stop = text.find(")");
	int len = stop - start;

	current_ip = text.mid(start, len);

	if (state)
		(void) new QListViewItem(mainView, current_ip, "", "Up", "ICMP", "Ping Reply");
}

/*
 * Adds a port entry to the scan window
 */
void KmPortView::handlePortEntry( const QString &text )
{
	if (text.isEmpty())
	{
		in_port_list = false;
		return;
	}

	// Split up first 4 entries and add to window
	QString tmp = text;
	QTextStream instream( tmp , IO_ReadOnly );
	QString w[4];

	for ( int i = 0 ; i < 4 ; i++ )
		instream >> w[i];

	(void) new QListViewItem(mainView, current_ip, w[0], w[1], w[2], w[3]);
}

void KmPortView::slotLogFile()
{
	pathEdit->setEnabled(saveBox->isChecked());
	browseBtn->setEnabled(saveBox->isChecked());
	saveBtn->setEnabled(saveBox->isChecked());
}
void KmPortView::slotBtnBrowse()
{
	QString text = QFileDialog::getSaveFileName();

	if (text.isEmpty())
		return;

	pathEdit->setText(text);

	KmConfig kmconf;
	kmconf.setPortViewLog(text);
}
void KmPortView::slotBtnSave()
{
	QString filename = pathEdit->text();

	if (filename.isEmpty()) {
		QMessageBox::information(0, i18n("Error: No filename given"),
		 	  i18n("No filename was given. Please choose a filename first."));
		return;
	}

	// Construct text to dump
	QString text;
	QListViewItem *item = mainView->firstChild();

	while (item)
	{
		text += item->text(0);
		text += " ";
		text += item->text(1);
		text += " ";
		text += item->text(2);
		text += " ";
		text += item->text(3);
		text += " ";
		text += item->text(4);
		text += "\n";

		item = item->itemBelow();
	}

	// Save it
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

/*
 * Redraws the column sizes mostly
 */
void KmPortView::paintEvent( QPaintEvent *event )
{
	this->QWidget::paintEvent( event );

	mainView->setGeometry( 0 , 0 , width() , height() - 35);	

	// Setup each column width
	int w_ipaddr = 110;
	int w_avg = (mainView->width() - w_ipaddr - 20) / 4; // 20 for scroll bar

	mainView->setColumnWidth( 0 , w_ipaddr );  // ip address
	mainView->setColumnWidth( 1 , w_avg );     // port number
	mainView->setColumnWidth( 2 , w_avg );     // state (open/filtered/closed)
	mainView->setColumnWidth( 3 , w_avg );     // protocal
	mainView->setColumnWidth( 4 , w_avg );     // service

	saveBox->setGeometry(5, height() - 30, 90, 25);
	pathEdit->setGeometry(100, height() - 30, width() - 300, 25);
	browseBtn->setGeometry(width() - 195, height() - 30, 90, 25);
	saveBtn->setGeometry(width() - 95, height() - 30, 90, 25);
}

/*
 * Asks the main kmap to hide me nicely
 */
void KmPortView::closeEvent( QCloseEvent *event )
{
	QWidget::closeEvent(event);

	// Tell hidden
	emit sigHidden();
}


























