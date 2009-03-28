/***************************************************************************
                          kmcommon.h  -  description
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

#ifndef KMCOMMON_H
#define KMCOMMON_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream.h>
#include <stdlib.h>

// include files for QT
#include <qobject.h>
#include <qwidget.h>

#include <qstring.h>
#include <qstrlist.h>
#include <qtextstream.h>
#include <qregexp.h>

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>

#include <qprinter.h>
#include <qpainter.h>
#include <qdatetime.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

#include <qlistview.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilinedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qtabdialog.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qcombobox.h>

#include <qevent.h>
#include <qsize.h>
#include <qpoint.h>

// include files for KDE
#include <kapp.h>
#include <kconfig.h>
#include <kprocess.h>
#include <kiconloader.h>
#include <ktmainwindow.h>
#include <ktoolbar.h>
#include <kmenubar.h>

// kde2 includes
#if KDE_VERSION_MAJOR >= 2 || KDE_VERSION_MINOR > 5
#include <klocale.h>
#endif

#endif /* KMCOMMON_H */