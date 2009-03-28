/***************************************************************************
                          kmoptstealth.cpp  -  description
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

// TODO: reformat member names

#include "kmoptstealth.h"
#include "kmconfig.h"

KmOptStealth::KmOptStealth(QWidget *parent, const char *name) : QWidget(parent,name) {
	btn_nmapdef = new QCheckBox(i18n("Nmap-defined modes"), this, "BtnNmapDef");
	btn_userdef = new QCheckBox(i18n("User-defined modes"), this, "BtnUserDef");

	w_canned = new QComboBox(this, "ComboCanned");
	w_canned->insertItem(i18n("Paranoid"));
	w_canned->insertItem(i18n("Sneaky"));
	w_canned->insertItem(i18n("Polite"));
	w_canned->insertItem(i18n("Normal"));
	w_canned->insertItem(i18n("Aggressive"));
	w_canned->insertItem(i18n("Insane"));

	w_host = new QCheckBox(i18n("Single host timeout (ms)"), this, "CbHost");
	w_max_rtt = new QCheckBox(i18n("Maximum retransmit timeout (ms)"), this, "CbMaxRtt");
	w_init_rtt = new QCheckBox(i18n("Initial retransmit timeout (ms)"), this, "CbInitRtt");
	w_parallel = new QCheckBox(i18n("Maximum # of parallel scans"), this, "CbParallel");
	w_delay = new QCheckBox(i18n("Minimum delay between probes (ms)"), this, "CbDelay");

	w_host_text = new QLineEdit(this, "");
	w_max_rtt_text = new QLineEdit(this, "");
	w_init_rtt_text = new QLineEdit(this, "");
	w_parallel_text = new QLineEdit(this, "");
	w_delay_text = new QLineEdit(this, "");

	connect(btn_nmapdef, SIGNAL(clicked()), SLOT(slotUpdate()));
	connect(btn_userdef, SIGNAL(clicked()), SLOT(slotUpdate()));

	connect(w_host, SIGNAL(clicked()), SLOT(slotHost()));
	connect(w_max_rtt, SIGNAL(clicked()), SLOT(slotMaxRtt()));
	connect(w_init_rtt, SIGNAL(clicked()), SLOT(slotInitRtt()));
	connect(w_parallel, SIGNAL(clicked()), SLOT(slotParallel()));
	connect(w_delay, SIGNAL(clicked()), SLOT(slotDelay()));

	LoadDefaults();
	Load();

	slotUpdate();
}
KmOptStealth::~KmOptStealth(){
}


QString KmOptStealth::getArguments()
{
	QString tmp = "";

	if (btn_nmapdef->isChecked())
	{
		if (w_canned->currentItem() == 0)
			tmp += "-T paranoid ";
		if (w_canned->currentItem() == 1)
			tmp += "-T sneaky ";
		if (w_canned->currentItem() == 2)
			tmp += "-T polite ";
		if (w_canned->currentItem() == 3)
			tmp += "-T normal ";
		if (w_canned->currentItem() == 4)
			tmp += "-T aggressive ";
		if (w_canned->currentItem() == 5)
			tmp += "-T insane ";
	}

	if (btn_userdef->isChecked())
	{
		if ((w_host->isChecked()) && (w_host_text->maxLength() > 0))
		{
			tmp += "--host_timeout ";
			tmp += w_host_text->text();
			tmp += " ";
		}

		if ((w_max_rtt->isChecked()) && (w_max_rtt_text->text() != ""))
		{
			tmp += "--max_rtt_timeout ";
			tmp += w_max_rtt_text->text();
			tmp += " ";
		}

		if ((w_init_rtt->isChecked()) && (w_init_rtt_text->text() != ""))
		{
			tmp += "--initial_rtt_timeout ";
			tmp += w_init_rtt_text->text();
			tmp += " ";
		}

		if ((w_parallel->isChecked()) && (w_parallel_text->text() != ""))
		{
			tmp += "--max_parallelism ";
			tmp += w_parallel_text->text();
			tmp += " ";
		}

		if ((w_delay->isChecked()) && (w_delay_text->text() != ""))
		{
			tmp += "--scan_delay ";
			tmp += w_delay_text->text();
			tmp += " ";
		}
	}

	// Save options
/*
	KConfig *config = kapp->getConfig();

	config->writeEntry("Stealth_Host", w_host_text->text());
	config->writeEntry("Stealth_Max_Rtt", w_max_rtt_text->text());
	config->writeEntry("Stealth_Init_Rtt", w_init_rtt_text->text());
	config->writeEntry("Stealth_Parallel", w_parallel_text->text());
	config->writeEntry("Stealth_Delay", w_delay_text->text());
*/
	return tmp;
}

void KmOptStealth::Load()
{
	KmConfig kmconf;

	QString text = kmconf.getArguments();
	QString parm = "";
	QTextStream is(text, IO_ReadOnly);

	while (!is.eof())
	{
		is >> parm;

		check_parms:
		if (parm == "-T")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;
			if (parm.isEmpty())
				continue;

			btn_nmapdef->setChecked(true);

			// Lowercase just to be sure
			parm = parm.lower();

			if ((parm == "0") || (parm == "paranoid"))
				w_canned->setCurrentItem(0);
			if ((parm == "1") || (parm == "sneaky"))
				w_canned->setCurrentItem(1);
			if ((parm == "2") || (parm == "polite"))
				w_canned->setCurrentItem(2);
			if ((parm == "3") || (parm == "normal"))
				w_canned->setCurrentItem(3);
			if ((parm == "4") || (parm == "aggressive"))
				w_canned->setCurrentItem(4);
			if ((parm == "5") || (parm == "insane"))
				w_canned->setCurrentItem(5);
		}

		// Lowercase just to be sure
		parm = parm.lower();

		if (parm == "--host_timeout")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;
			if (parm.isEmpty())
				continue;

			btn_userdef->setChecked(true);

			w_host->setChecked(true);
			w_host_text->setText(parm);

			slotHost();
		}
		
		if (parm == "--max_rtt_timeout")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;
			if (parm.isEmpty())
				continue;

			btn_userdef->setChecked(true);

			w_max_rtt->setChecked(true);
			w_max_rtt_text->setText(parm);

			slotMaxRtt();
		}
		
		if (parm == "--initial_rtt_timeout")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;
			if (parm.isEmpty())
				continue;

			btn_userdef->setChecked(true);

			w_init_rtt->setChecked(true);
			w_init_rtt_text->setText(parm);

			slotInitRtt();
		}
		
		if (parm == "--max_parallelism")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;
			if (parm.isEmpty())
				continue;

			btn_userdef->setChecked(true);

			w_parallel->setChecked(true);
			w_parallel_text->setText(parm);

			slotParallel();
		}
		
		if (parm == "--scan_delay")
		{
			is >> parm;
			
			if (parm.left(1) == "-")
				goto check_parms;
			if (parm.isEmpty())
				continue;

			btn_userdef->setChecked(true);

			w_delay->setChecked(true);
			w_delay_text->setText(parm);

			slotDelay();
		}
	}

	slotUpdate();
}
void KmOptStealth::LoadDefaults()
{
	btn_nmapdef->setChecked(false);
	btn_userdef->setChecked(false);

	w_canned->setCurrentItem(3); // normal

	w_host->setChecked(false);
	w_max_rtt->setChecked(false);
	w_init_rtt->setChecked(false);
	w_parallel->setChecked(false);
	w_delay->setChecked(false);

	w_host_text->clear();
	w_max_rtt_text->clear();
	w_init_rtt_text->clear();
	w_parallel_text->clear();
	w_delay_text->clear();

	slotUpdate();
}

void KmOptStealth::slotUpdate()
{
	w_canned->setEnabled(btn_nmapdef->isChecked());

	w_host->setEnabled(btn_userdef->isChecked());
	w_max_rtt->setEnabled(btn_userdef->isChecked());
	w_init_rtt->setEnabled(btn_userdef->isChecked());
	w_parallel->setEnabled(btn_userdef->isChecked());
	w_delay->setEnabled(btn_userdef->isChecked());

	if (btn_userdef->isChecked())
	{
		slotHost();
		slotMaxRtt();
		slotInitRtt();
		slotParallel();
		slotDelay();
	}
	else
	{
		w_host_text->setEnabled(false);
		w_max_rtt_text->setEnabled(false);
		w_init_rtt_text->setEnabled(false);
		w_parallel_text->setEnabled(false);
		w_delay_text->setEnabled(false);
	}
}

void KmOptStealth::slotHost()
{
	w_host_text->setEnabled(w_host->isChecked());
}
void KmOptStealth::slotMaxRtt()
{
	w_max_rtt_text->setEnabled(w_max_rtt->isChecked());
}
void KmOptStealth::slotInitRtt()
{
	w_init_rtt_text->setEnabled(w_init_rtt->isChecked());
}
void KmOptStealth::slotParallel()
{
	w_parallel_text->setEnabled(w_parallel->isChecked());
}
void KmOptStealth::slotDelay()
{
	w_delay_text->setEnabled(w_delay->isChecked());
}

void KmOptStealth::paintEvent(QPaintEvent *event)
{
	this->QWidget::paintEvent(event);

	btn_nmapdef->setGeometry(10, 10, width() - 170, 25);
	w_canned->setGeometry(width() - 160, 5, 150, 25);

	btn_userdef->setGeometry(10, 40, width() - 20, 25);

	w_host->setGeometry(30, 70, width() - 150, 25);
	w_host_text->setGeometry(width() - 100, 70, 70, 25);

	w_max_rtt->setGeometry(30, 100, width() - 150, 25);
	w_max_rtt_text->setGeometry(width() - 100, 100, 70, 25);

	w_init_rtt->setGeometry(30, 130, width() - 150, 25);
	w_init_rtt_text->setGeometry(width() - 100, 130, 70, 25);

	w_parallel->setGeometry(30, 160, width() - 150, 25);
	w_parallel_text->setGeometry(width() - 100, 160, 70, 25);

	w_delay->setGeometry(30, 190, width() - 150, 25);
	w_delay_text->setGeometry(width() - 100, 190, 70, 25);
}



