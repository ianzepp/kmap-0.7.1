/***************************************************************************
                          kminfoview.cpp  -  description
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

#include "kminfoview.h"
#include "kmconfig.h"

KmInfoView::KmInfoView(QWidget *parent, const char *name): QWidget(parent,name){
	this->setCaption(i18n("Kmap: Information"));

	pix = new QPixmap();

	mainView = new QListView(this);
	mainView->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	mainView->setAllColumnsShowFocus(true);
	mainView->setRootIsDecorated(true);
	mainView->setSorting(0, true);

	mainView->addColumn(i18n("The Problem"));
	mainView->addColumn(i18n("The Solution"));

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
	pathEdit->setText(kmconf.getInfoViewLog());

}
KmInfoView::~KmInfoView(){
}

void KmInfoView::Clear()
{
	mainView->clear(); // clears listview

	i_cnt = 0;
	in_port_list = false;
	in_fingerprint = false;

	current_ip = "";
	current_host = "";

	tcp_port_list = "";
	udp_port_list = "";
	misc_port_list = "";

	our_problem = "";
	our_solution = "";

	print_text = "";
}

void KmInfoView::slotStarted()
{
	Clear();
}

void KmInfoView::slotStdout(const QString &text)
{
	if (in_port_list)
	{
		// This has to be first, because this function will flush
		// outstanding info when done getting data
		handlePortEntry(text);

		if (text.isEmpty())
			in_port_list = false;
		return; // no more parsing allowed here
	}

	if (in_fingerprint)
	{
		// This has to be first, because this function will flush
		// outstanding info when done getting data
		handlePrintEntry(text);

		if (text.isEmpty())
			in_fingerprint = false;
		return; // no more parsing allowed here
	}

	// Search for middle of line strings first
	if (text.find(QRegExp("Class=")) != -1)
		handleClass(text); // in middle of line

	if (text.find(QRegExp("Difficulty=")) != -1)
		handleDifficulty(text); // in middle of line

	// Search for start of line strings next
	if (text.find(QRegExp("^Port ")) != -1)
		in_port_list = true;

	if (text.find(QRegExp("^TCP/IP fingerprint:")) != -1)
		in_fingerprint = true;

	if (text.find(QRegExp("^No OS matches for host ")) != -1)
		handleUnknownOS(text);

	if (text.find(QRegExp("^Interesting ports on ")) != -1)
		handleIpAddress(text);

	if (text.find(QRegExp("^Host [a-zA-Z\\.]+")) != -1)
		handleIpAddress2(text);

	if (text.find(QRegExp("^Sequence numbers:")) != -1)
		handleSequence(text);

	if (text.find(QRegExp("^Remote operating system guess:")) != -1)
		handleRemoteOS(text);

	if (text.find(QRegExp("^Remote OS guesses:")) != -1)
		handleRemoteOS(text);

}
void KmInfoView::slotStderr(const QString &text)
{
}
void KmInfoView::slotExited()
{
}

void KmInfoView::handlePortEntry(const QString &text)
{
	if (text.isEmpty())// handle this first
	{
		if (!tcp_port_list.isEmpty())
		{
			our_problem  = i18n("Open TCP ports:");
			our_solution = tcp_port_list;

			(void) new QListViewItem(p_currentip, our_problem, our_solution);
		}

		if (!udp_port_list.isEmpty())
		{
			our_problem  = i18n("Open UDP ports:");
			our_solution = udp_port_list;

			(void) new QListViewItem(p_currentip, our_problem, our_solution);
		}

		if (!misc_port_list.isEmpty())
		{
			our_problem  = i18n("Unknown-type ports:");
			our_solution = misc_port_list;

			(void) new QListViewItem(p_currentip, our_problem, our_solution);
		}

		in_port_list = false;

		return;
	}

	QString port = text.left(text.find(" "));

	if (text.find(QRegExp("tcp")) != -1)
		tcp_port_list += port + ",";
	else if (text.find(QRegExp("udp")) != -1)
		udp_port_list += port + ",";
	else
		misc_port_list += port + ",";

}
void KmInfoView::handlePrintEntry(const QString &text)
{
	if (text.isEmpty()) // end of print
	{
		KmConfig::appendFingerPrint(print_text);
		return;
	}

	// do more stuff
	if (print_text.isEmpty())
	{
		print_text += "Fingerprint generated at: ";
		print_text += QDate::currentDate().toString();
		print_text += "\n";

		print_text += "Remote host is: ";
		print_text += current_ip;
		print_text += " (";
		print_text += current_host;
		print_text += ")\n";
	}
	
	// Add line of text
	print_text += text + "\n";
}
void KmInfoView::handleIpAddress(const QString &text)
{
	int start = text.find("(") + 1;
	int stop = text.find(")");
	int len = stop - start;

	current_ip = text.mid(start, len);
	tcp_port_list = "";
	udp_port_list = "";
	misc_port_list = "";

	// Name to problem
	our_problem = current_ip;

	// Try to extract a hostname
	our_solution = text.left(start - 1);
	our_solution.replace(QRegExp("^Interesting ports on "), "");
	our_solution.replace(QRegExp("\\s"), "");

	if (our_solution.isEmpty())
		our_solution = i18n("IP address did not resolve");
	else
		our_solution.prepend(i18n("IP address resolved to: "));

	// Set current ip address and add in ..
	p_currentip = new QListViewItem(mainView, our_problem, our_solution);
}
void KmInfoView::handleIpAddress2(const QString &text)
{
	// Check and make sure that this is a ping scan first. Nmap will
	// also push out this line if you have verbose turned on, in which
	// case we will get bouble entries for each host.
	KmConfig kmconf;
	QString args = kmconf.getArguments();
	if (args.find("-sP") == -1)
		return;

	// Normal stuff
	int start = text.find("(") + 1;
	int stop = text.find(")");
	int len = stop - start;

	current_ip = text.mid(start, len);
	tcp_port_list = "";
	udp_port_list = "";
	misc_port_list = "";

	// Name to problem
	our_problem = current_ip;

	// Try to extract a hostname
	our_solution = text.left(start - 1);
	our_solution.replace(QRegExp("^Host "), "");
	our_solution.replace(QRegExp("\\s"), "");

	if (our_solution.isEmpty())
		our_solution = i18n("IP address did not resolve");
	else
		our_solution.prepend(i18n("IP address resolved to: "));

	// Set current ip address and add in ..
	p_currentip = new QListViewItem(mainView, our_problem, our_solution);
}
void KmInfoView::handleClass(const QString &text)
{
	our_problem = i18n("Sequence class: ");
	our_solution = text.mid(text.find("=") + 1, text.length());
	our_solution.stripWhiteSpace();

	(void) new QListViewItem(p_currentip, our_problem, our_solution);
}
void KmInfoView::handleDifficulty(const QString &text)
{
	our_problem = i18n("Sequence difficulty: ");
	our_solution = text.mid(text.find("=") + 1, text.length());
	our_solution.stripWhiteSpace();

	(void) new QListViewItem(p_currentip, our_problem, our_solution);
}
void KmInfoView::handleSequence(const QString &text)
{
	our_problem = i18n("Sequence numbers: ");
	our_solution = text.mid(text.find(":") + 1, text.length());
	our_solution.stripWhiteSpace();

	(void) new QListViewItem(p_currentip, our_problem, our_solution);
}
void KmInfoView::handleRemoteOS(const QString &text)
{
	our_problem = i18n("Remote OS: ");
	our_solution = text.mid(text.find(":") + 1, text.length());
	our_solution.stripWhiteSpace();

	(void) new QListViewItem(p_currentip, our_problem, our_solution);
}
void KmInfoView::handleUnknownOS(const QString &text)
{
	our_problem = i18n("Unknown OS: ");
	our_solution = i18n("No OS matches for host");
	our_solution.stripWhiteSpace();

	(void) new QListViewItem(p_currentip, our_problem, our_solution);
}


void KmInfoView::slotLogFile()
{
	pathEdit->setEnabled(saveBox->isChecked());
	browseBtn->setEnabled(saveBox->isChecked());
	saveBtn->setEnabled(saveBox->isChecked());
}
void KmInfoView::slotBtnBrowse()
{
	QString text = QFileDialog::getSaveFileName();

	if (text.isEmpty())
		return;

	pathEdit->setText(text);

	KmConfig kmconf;
	kmconf.setInfoViewLog(text);
}
void KmInfoView::slotBtnSave()
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

void KmInfoView::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	int w_prob = 150;

	mainView->setGeometry(0, 0, width(), height() - 35);	
	mainView->setColumnWidth(0, w_prob);
	mainView->setColumnWidth(1, mainView->width() - w_prob - 20);

	saveBox->setGeometry(5, height() - 30, 90, 25);
	pathEdit->setGeometry(100, height() - 30, width() - 300, 25);
	browseBtn->setGeometry(width() - 195, height() - 30, 90, 25);
	saveBtn->setGeometry(width() - 95, height() - 30, 90, 25);
}

/*
 * Asks the main kmap to hide me nicely
 */
void KmInfoView::closeEvent( QCloseEvent *event )
{
	QWidget::closeEvent(event);

	// Tell hidden
	emit sigHidden();
}
























