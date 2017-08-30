/***************************************************************************
                               jspice.cpp
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


#include "jspice.h"
#include "components/equation.h"
#include "main.h"

/*!
  \file jspice.cpp
  \brief Implementation of the JSpice class
*/

/*!
 * \brief JSpice::JSpice Class constructor
 * \param sch_ Schematic that need to be simualted with JSpice.
 * \param parent Parent object
 */
JSpice::JSpice(Schematic *sch_, QObject *parent) :
    AbstractSpiceKernel(sch_, parent)
{
    simulator_cmd = QucsSettings.JSpiceExecutable;
    Nprocs = QucsSettings.NProcs;
    Noisesim = false;
}

/*!
 * \brief JSpice::determineUsedSimulations Determine simulation used
 *        in schematic and add them into simulationsQueue list
 */
void JSpice::determineUsedSimulations()
{

    for(Component *pc = Sch->DocComps.first(); pc != 0; pc = Sch->DocComps.next()) {
       if(pc->isSimulation) {
           QString sim_typ = pc->Model;
           if (sim_typ==".AC") simulationsQueue.append("ac");
           if (sim_typ==".NOISE") simulationsQueue.append("noise");
           if (sim_typ==".TR") simulationsQueue.append("tran");
           if (sim_typ==".JSSCR") simulationsQueue.append(pc->Name); // May be >= JSpice scripts
           if ((sim_typ==".SW")&&
               (pc->Props.at(0)->Value.startsWith("DC"))) simulationsQueue.append("dc");
       }
    }
}

/*!
 * \brief JSpice::createNetlist
 * \param[out] stream QTextStream that associated with spice netlist file
 * \param[in] simulations The list of simulations that need to included in netlist.
 * \param[out] vars The list of output variables and node names.
 * \param[out] outputs The list of spice output raw text files.
 */
void JSpice::createNetlist(QTextStream &stream, int , QStringList &simulations,
                    QStringList &vars, QStringList &outputs)
{
    QString s;
    bool hasParSweep = false;

    if(!prepareSpiceNetlist(stream)) return; // Unable to perform spice simulation
    startNetlist(stream,true);

    // set variable names for named nodes and wires
    vars.clear();
    for(Node *pn = Sch->DocNodes.first(); pn != 0; pn = Sch->DocNodes.next()) {
        if(pn->Label != 0) {
            if (!vars.contains(pn->Label->Name)) {
                vars.append(pn->Label->Name);
            }
        }
    }
    for(Wire *pw = Sch->DocWires.first(); pw != 0; pw = Sch->DocWires.next()) {
        if(pw->Label != 0) {
            if (!vars.contains(pw->Label->Name)) {
                vars.append(pw->Label->Name);
            }
        }
    }
    for(Component *pc = Sch->DocComps.first(); pc != 0; pc = Sch->DocComps.next()) {
        if (pc->isProbe) {
            QString var_pr = pc->getProbeVariable(true);
            if (!vars.contains(var_pr)) {
                vars.append(var_pr);
            }
        }
        /*if (pc->isEquation) {
            Equation *eq = (Equation *)pc;
            QStringList vars_eq;
            eq->getDepVars(vars_eq);
            vars.append(vars_eq);
        }*/
    }

    if (DC_OP_only) {
        // Add all remaining nodes, because JSpice has no equivalent for PRINT ALL
        for(Node* pn = Sch->Nodes->first(); pn != 0; pn = Sch->Nodes->next()) {
            if ((!vars.contains(pn->Name))&&(pn->Name!="gnd")) {
                vars.append(pn->Name);
            }
        }
        // Add DC sources
        for(Component *pc = Sch->DocComps.first(); pc != 0; pc = Sch->DocComps.next()) {
            if ((pc->Model == "S4Q_V")||(pc->Model == "Vdc")) {
                vars.append("I("+pc->Name+")");
            }
        }
    }

    vars.sort();

    //execute simulations

    QFileInfo inf(Sch->DocName);
    QString basenam = inf.baseName();

    QString nod,nods;
    nods.clear();
    foreach (nod,vars) {
        if (!nod.startsWith("I(")) {
            nods += QString("v(%1) ").arg(nod);
        } else {
            nods += nod + " ";
        }
    }

    if (DC_OP_only) {
        stream<<".OP\n";
        stream<<QString(".PRINT dc format=noindex file=spice4qucs.cir.dc_op_jspice %1\n").arg(nods);
        outputs.append("spice4qucs.cir.dc_op_jspice");
        return;
    }

    QString sim = simulations.first();

    for(Component *pc = Sch->DocComps.first(); pc != 0; pc = Sch->DocComps.next()) { // JSpice can run
       if(pc->isSimulation) {                        // only one simulations per time.
           QString sim_typ = pc->Model;              // Multiple simulations are forbidden.
           QString s = pc->getSpiceNetlist(true);
           if ((sim_typ==".AC")&&(sim=="ac")) stream<<s;
           if ((sim_typ==".NOISE")&&(sim=="noise")) stream<<s;
           if (sim==pc->Name) stream<<s; // JSpice scripts
           if ((sim_typ==".TR")&&(sim=="tran")){
               stream<<s;
               Q3PtrList<Component> comps(Sch->DocComps); // find Fourier tran
               for(Component *pc1 = comps.first(); pc1 != 0; pc1 = comps.next()) {
                   if (pc1->Model==".FOURIER") {
                      if (pc1->Props.at(0)->Value==pc->Name) {
                           QString s1 = pc1->getSpiceNetlist(true);
                           outputs.append("spice4qucs.tran.cir.four");
                           stream<<s1;
                       }
                    }
                }
            }
            if (sim_typ==".SW") {
                QString SwpSim = pc->Props.at(0)->Value;
                if (SwpSim.startsWith("DC")&&(sim=="dc")) stream<<s;
                else if (SwpSim.startsWith("AC")&&(sim=="ac")) {
                    stream<<s;
                    hasParSweep = true;
                }
                else if (SwpSim.startsWith("TR")&&(sim=="tran")) {
                    stream<<s;
                    hasParSweep = true;
                }    
                if (SwpSim.startsWith("SW")&&(sim=="dc")) {
                    for(Component *pc1 = Sch->DocComps.first(); pc1 != 0; pc1 = Sch->DocComps.next()) {
                        if ((pc1->Name==SwpSim)&&(pc1->Props.at(0)->Value.startsWith("DC"))) {
                            stream<<s;
                        hasParSweep = true;
                        }
                    }
                }
            }
        if ((sim_typ==".DC")) stream<<s;
        }
    }

    if (sim.startsWith("JSSCR")) {
        for(Component *pc = Sch->DocComps.first(); pc != 0; pc = Sch->DocComps.next()) {
            if (pc->isSimulation)
                if (sim == pc->Name)
                    outputs.append(pc->Props.at(2)->Value.split(';'));
        }
        stream<<".END\n";
        return;
    }

    QString filename;
    if (hasParSweep) filename = QString("%1_%2_swp.txt").arg(basenam).arg(sim);
    else filename = QString("%1_%2.txt").arg(basenam).arg(sim);
    filename.remove(QRegExp("\\s")); // JSpice doesn't support spaces and quotes
    QString write_str;
    
    if (sim=="noise") {
        filename += "_std";
        write_str = QString(".PRINT noise file=%1 inoise onoise\n").arg(filename);
        outputs.append(filename);
    } else {
        write_str = QString(".PRINT  %1 format=raw file=%2 %3\n").arg(sim).arg(filename).arg(nods);
        outputs.append(filename);
    }
    stream<<write_str;

    stream<<".END\n";
}

/*!
 * \brief JSpice::slotSimulate Execute JSpice simualtor and perform all
 *        simulations from the simulationQueue list
 */
void JSpice::slotSimulate()
{

    QStringList incompat;
    if (!checkSchematic(incompat)) {
        QString s = incompat.join("; ");
        output.append("There were SPICE-incompatible components. Simulator cannot proceed.");
        output.append("Incompatible components are: " + s + "\n");
        emit finished();
        emit errors(QProcess::FailedToStart);
        return;
    }

    int num=0;
    netlistQueue.clear();
    output_files.clear();

    if (DC_OP_only) {
        simulationsQueue.append("dc");
    } else  determineUsedSimulations();

    foreach(QString sim,simulationsQueue) {
        QStringList sim_lst;
        sim_lst.clear();
        sim_lst.append(sim);
        QString tmp_path = QDir::convertSeparators(workdir+"/spice4qucs."+sim+".cir");
        netlistQueue.append(tmp_path);
        QFile spice_file(tmp_path);
        if (spice_file.open(QFile::WriteOnly)) {
            QTextStream stream(&spice_file);
            createNetlist(stream,num,sim_lst,vars,output_files);
            spice_file.close();
        }
    }

    output.clear();
    emit started();
    nextSimulation();
}

/*!
 * \brief JSpice::SaveNetlist Save netlist into specified file without
 *        execution of simulator.
 * \param[in] filename The name of file in which netlist is saved
 */
void JSpice::SaveNetlist(QString filename)
{
    determineUsedSimulations();
    int num = 0;
    QFile spice_file(filename);
    if (spice_file.open(QFile::WriteOnly)) {
        QTextStream stream(&spice_file);
        createNetlist(stream,num,simulationsQueue,vars,output_files);
        spice_file.close();
    }
}

/*!
 * \brief JSpice::slotFinished Simualtor finished handler. End simulation or
 *        execute the next simulation from queue.
 */
void JSpice::slotFinished()
{ 
    output += SimProcess->readAllStandardOutput();;

    if (Noisesim) {
        QFile logfile(workdir + QDir::separator() + "spice4qucs.noise_log");
        if (logfile.open(QIODevice::WriteOnly)) {
            QTextStream ts(&logfile);
            ts<<output;
            logfile.close();
        }
        Noisesim = false;
        output_files.append("spice4qucs.noise_log");
    }

    if (netlistQueue.isEmpty()) {
        emit finished();
        emit progress(100);
        return;
    } else {
        nextSimulation();
    }
}

bool JSpice::waitEndOfSimulation()
{
    bool ok = false;
    while (!netlistQueue.isEmpty()) {
        ok = SimProcess->waitForFinished(10000);
    }
    ok = SimProcess->waitForFinished(10000);
    return ok;
}

/*!
 * \brief JSpice::slotProcessOutput Process JSpice output and report progress.
 */
void JSpice::slotProcessOutput()
{
    //***** Percent complete: 85.4987 %
    QString s = SimProcess->readAllStandardOutput();
    if (s.contains("Percent complete:")) {
        int percent = round(s.section(' ',3,3,QString::SectionSkipEmpty).toFloat());
        emit progress(percent);
    }
    output += s;
}

/*!
 * \brief JSpice::nextSimulation Execute the next simulation from queue.
 */
void JSpice::nextSimulation()
{
    if (!netlistQueue.isEmpty()) {
        QString file = netlistQueue.takeFirst();
        if (file.endsWith(".noise.cir")) Noisesim = true;
        SimProcess->setWorkingDirectory(workdir);
        QString cmd = QString("%1 %2 \"%3\"").arg(simulator_cmd,simulator_parameters,file);
        SimProcess->start(cmd);
    } else {
        output += "No simulations!\n"
                  "Exiting...\n";
        emit progress(100);
        emit finished(); // nothing to simulate
    }
}
