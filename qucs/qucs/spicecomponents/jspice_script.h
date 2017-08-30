/***************************************************************************
                               jspice_script.h
                                ----------
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

#ifndef SP_JSPICESCRIPT_H
#define SP_JSPICESCRIPT_H

#include "components/component.h"


class JSpiceScript : public Component  {
public:
  JSpiceScript();
  ~JSpiceScript();
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

protected:
  QString spice_netlist(bool isXyce = false);
};

#endif
