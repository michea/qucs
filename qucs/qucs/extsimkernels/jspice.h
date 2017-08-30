/***************************************************************************
                               jspice.h
                             ----------------
    begin                : Fri Jun 30 2017
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

#ifndef JSPICE_H
#define JSPICE_H

#include <QtCore>
#include "abstractspicekernel.h"

/*!
  \file jspice.h
  \brief Declaration of the JSpice class
*/

/*!
 * \brief The JSpice class responsible for execution of JSpice simulator.
 */
class JSpice : public AbstractSpiceKernel
{
    Q_OBJECT
private:
    bool Noisesim;

    unsigned int Nprocs;
    QStringList simulationsQueue;
    QStringList netlistQueue;
    void determineUsedSimulations();
    void nextSimulation();

public:
    explicit JSpice(Schematic *sch_, QObject *parent = 0);

    void SaveNetlist(QString filename);
    bool waitEndOfSimulation();
    
protected:
    void createNetlist(QTextStream &stream, int NumPorts, QStringList &simulations,
                  QStringList &vars, QStringList &outputs);
protected slots:
    void slotFinished();
    void slotProcessOutput();

public slots:
    void slotSimulate();
    
};

#endif // JSPICE_H
