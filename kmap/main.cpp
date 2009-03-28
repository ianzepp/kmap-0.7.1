/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Fri Jan  7 00:40:23 EST 2000
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

#include "kmap.h"
 
int main(int argc, char *argv[])
{
  KApplication a(argc, argv, "kmap");
  
  Kmap *kmap = new Kmap();
  a.setMainWidget(kmap);
  kmap->show();

  return a.exec();
}
