/***************************************************************************
                            simsettingsdialog.h
                             ----------------
    begin                : Tue Apr 21 2015
    copyright            : (C) 2015 by Vadim Kuznetsov
    email                : ra3xdh@gmail.com
    modified             : Fri Jun 26, 2017 by Michael Arendall - added JSpice
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


#ifndef SIMSETTINGSDIALOG_H
#define SIMSETTINGSDIALOG_H

#include <QtGui>

class SimSettingsDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel *lblXyce;
    QLabel *lblXycePar;
    QLabel *lblJSpice;
    QLabel *lblNgspice;
    QLabel *lblSpiceOpus;
    QLabel *lblNprocs;
    QLabel *lblWorkdir;
    QLabel *lblSimulator;

    QComboBox *cbxSimulator;

    QLineEdit *edtNgspice;
    QLineEdit *edtSpiceOpus;
    QLineEdit *edtXyce;
    QLineEdit *edtXycePar;
    QLineEdit *edtJSpice;
    QSpinBox  *spbNprocs;
    QLineEdit *edtWorkdir;

    QPushButton *btnOK;
    QPushButton *btnCancel;

    QPushButton *btnSetNgspice;
    QPushButton *btnSetSpOpus;
    QPushButton *btnSetXyce;
    QPushButton *btnSetXycePar;
    QPushButton *btnSetJSpice;
    QPushButton *btnSetWorkdir;

public:
    explicit SimSettingsDialog(QWidget *parent = 0);
    
    
private slots:
    void slotApply();
    void slotCancel();
    void slotSetNgspice();
    void slotSetXyce();
    void slotSetXycePar();
    void slotSetJSpice();
    void slotSetSpiceOpus();
    void slotSetWorkdir();
    
};

#endif // SIMSETTINGSDIALOG_H
