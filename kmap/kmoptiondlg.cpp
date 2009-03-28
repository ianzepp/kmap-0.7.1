/***************************************************************************
                          kmoptiondlg.cpp  -  description
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

#include "kmoptiondlg.h"
#include "kmconfig.h"

#include "kmoptscans.h"
#include "kmoptother.h"
#include "kmoptdecoy.h"
#include "kmoptstealth.h"
#include "kmoptabout.h"
#include "kmoptpaths.h"

KmOptionDlg::KmOptionDlg(QWidget *parent, const char *name ) : QTabDialog(parent,name,true) {
	// Set buttons
	setOkButton();
	setDefaultButton();
	setCancelButton();

	// Connect to slots
	connect(this, SIGNAL(applyButtonPressed()), SLOT(slotApply()) );
	connect(this, SIGNAL(defaultButtonPressed()), SLOT(slotDefault()) );
	connect(this, SIGNAL(cancelButtonPressed()), SLOT(slotCancel()) );

	// Load Option views
	opt_scans = new KmOptScans(this);
	addTab(opt_scans, i18n("Scan Types"));

	opt_other = new KmOptOther(this);
	addTab(opt_other, i18n("Other Options"));

	opt_decoy = new KmOptDecoy(this);
	addTab(opt_decoy, i18n("Decoys"));

	opt_stealth = new KmOptStealth(this);
	addTab(opt_stealth, i18n("Stealth"));

	opt_paths = new KmOptPaths(this);
	addTab(opt_paths, i18n("Paths"));

	opt_about = new KmOptAbout(this);
	addTab(opt_about, i18n("About"));
}
KmOptionDlg::~KmOptionDlg(){
}

/*
 * Saves to settings
 */
void KmOptionDlg::slotApply()
{
	KmConfig kmconf;

	// Gather options
	QString tmp;
	tmp += opt_scans->getArguments();
	tmp += opt_other->getArguments();
	tmp += opt_decoy->getArguments();
	tmp += opt_stealth->getArguments();
	tmp += opt_paths->getArguments();

	kmconf.setArguments(tmp);

	emit sigReloadArguments();
}

/*
 * Resets all defaults
 */
void KmOptionDlg::slotDefault()
{
	// Tell them to reset to defaults
	opt_scans->LoadDefaults();
	opt_other->LoadDefaults();
	opt_decoy->LoadDefaults();
	opt_stealth->LoadDefaults();
	opt_paths->LoadDefaults();
}

/*
 * Cancels changes and exits
 */
void KmOptionDlg::slotCancel()
{
}
