/***************************************************************************
                         MEMRISTOR_SPICE.cpp  -  description
                   --------------------------------------
    begin                    : Thur Jan 19 2077
    copyright              : (C) 2017 by Michael Arendall
    email                     : marendall@nspace.com
    spice4qucs code added  Thur. 19 Jan 2017
    copyright              : (C) 2017 by Michael Arendall
    email                    : marendall@nspace.com

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "MEMRISTOR_SPICE.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"


MEMRISTOR_SPICE::MEMRISTOR_SPICE()
{
    Description = QObject::tr("SPICE MR:\nMultiple line Xyce YMEMRISTOR specifications allowed using \"+\" continuation lines.\nLeave continuation lines blank when NOT in use.  ");

    Lines.append(new Line( -30,  0, -20, 0,QPen(Qt::darkBlue,3)));
    Lines.append(new Line( 20, 0, 30, 0,QPen(Qt::darkBlue,3)));
    Lines.append(new Line( 20, -10, 20, 0,QPen(Qt::darkGreen,3)));
    Lines.append(new Line( 20, 0, 20, 10,QPen(Qt::darkGreen,3)));
    
    Arcs.append(new Arc( -20, -5, 20, 10, 0, 16*360, QPen(Qt::darkGreen,3)));
    Arcs.append(new Arc( 0, -5, 20, 10, 0, 16*360, QPen(Qt::darkGreen,3)));
    
    Ports.append(new Port(-30,  0));
    Ports.append(new Port( 30,  0));

    x1 = -30; y1 = -11;
    x2 =  30; y2 = 11;

    tx = x1+4;
    ty = y2+4;

    Model = "MEMRISTOR";
    SpiceModel = "YMEMRISTOR ";
    Name  = "MR";

    Props.append(new Property("MEMRISTOR", "", true,"MEMRISTOR param list and\n .model spec."));
    Props.append(new Property("MEMRISTOR_Line 2", "", false,"+ continuation line 1"));
    Props.append(new Property("MEMRISTOR_Line 3", "", false,"+ continuation line 2"));
    Props.append(new Property("MEMRISTOR_Line 4", "", false,"+ continuation line 3"));
    Props.append(new Property("MEMRISTOR_Line 5", "", false,"+ continuation line 4"));



    //rotate();  // fix historical flaw
}

MEMRISTOR_SPICE::~MEMRISTOR_SPICE()
{
}

Component* MEMRISTOR_SPICE::newOne()
{
  return new MEMRISTOR_SPICE();
}

Element* MEMRISTOR_SPICE::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr(" MR Memristor");
  BitmapFile = (char *) "MEMRISTOR_SPICE";

  if(getNewOne)  return new MEMRISTOR_SPICE();
  return 0;
}

QString MEMRISTOR_SPICE::netlist()
{
    return QString("");
}

QString MEMRISTOR_SPICE::spice_netlist(bool)
{
    QString s = spicecompat::check_refdes(Name,SpiceModel);
    foreach(Port *p1, Ports) {
        QString nam = p1->Connection->Name;
        if (nam=="gnd") nam = "0";
        s += " "+ nam+" ";   // node names
    }

    QString MEMRISTOR= Props.at(0)->Value;
    QString MEMRISTOR_Line_2= Props.at(1)->Value;
    QString MEMRISTOR_Line_3= Props.at(2)->Value;
    QString MEMRISTOR_Line_4= Props.at(3)->Value;
    QString MEMRISTOR_Line_5= Props.at(4)->Value;

    if(  MEMRISTOR.length()  > 0)          s += QString("%1\n").arg(MEMRISTOR);
    if(  MEMRISTOR_Line_2.length() > 0 )   s += QString("%1\n").arg(MEMRISTOR_Line_2);
    if(  MEMRISTOR_Line_3.length() > 0 )   s += QString("%1\n").arg(MEMRISTOR_Line_3);
    if(  MEMRISTOR_Line_4.length() > 0 )   s += QString("%1\n").arg(MEMRISTOR_Line_4);
    if(  MEMRISTOR_Line_5.length() > 0)    s += QString("%1\n").arg(MEMRISTOR_Line_5);
 
    return s;
}
