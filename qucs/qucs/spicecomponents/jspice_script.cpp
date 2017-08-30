/***************************************************************************
                              jspice_script.cpp
                               ------------
    begin                : Sat Jul 22 2017
    copyright            : (C) 2017 by Michael Arendall
    email                : miche.arendall@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "jspice_script.h"
#include "main.h"
#include "misc.h"


JSpiceScript::JSpiceScript()
{
  isSimulation = true;
  Description = QObject::tr("JSPICE script");

  Texts.append(new Text(0, 0, Description, Qt::darkRed, QucsSettings.largeFontSize));

  x1 = -10; y1 = -9;
  x2 = x1+104; y2 = y1+59;

  tx = 0;
  ty = y2+1;
  Model = ".JSSCR";
  Name  = "JSSCR";
  SpiceModel = "JSSCR";

  // The index of the first 4 properties must not changed. Used in recreate().
  Props.append(new Property("SpiceCode", "\n"
                            ".TRAN 1ns 1s \n"
                            ".PRINT TRAN format=raw file=tran.txt V(1)", true,
                                         "Insert spice code here"));
  Props.append(new Property("","",false,"Vars to plot"));
  Props.append(new Property("Outputs","tran.txt",false,"Extra outputs to parse"));

}

JSpiceScript::~JSpiceScript()
{
}

Component* JSpiceScript::newOne()
{
  return new JSpiceScript();
}

Element* JSpiceScript::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("JSPICE script");
  BitmapFile = (char *) "jspice_script";

  if(getNewOne)  return new JSpiceScript();
  return 0;
}

QString JSpiceScript::spice_netlist(bool isXyce)
{
    QString s = "";
    if (!isXyce) return s;
    s = Props.at(0)->Value+"\n";
    return s;
}
