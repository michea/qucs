/***************************************************************************
                          MEMRISTOR_SPICE.h  -  description
                      --------------------------------------
    begin                    : Wed Jam 19 2017
    copyright              : (C) 2017 by Michael Arendall
    email                     : mrendall@nspace.comboSim
    spice4qucs code added  Wed. 19 Jan 2017
    copyright              : (C) 2015 by Michael Arendall
    email                    : marendall@ynspace.com

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MEMRISTOR_SPICE_H
#define MEMRISTOR_SPICE_H

#include "components/component.h"

class MEMRISTOR_SPICE : public Component {
public:
  MEMRISTOR_SPICE();
  ~MEMRISTOR_SPICE();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);
protected:
  QString netlist();
  QString spice_netlist(bool isXyce = false);
};

#endif // MEMRISTOR_SPICE_H
