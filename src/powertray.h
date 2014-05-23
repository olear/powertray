/*
# PowerTray - Manage Power States on Dracolinux
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
#
# PowerTray is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 2.
*/


#ifndef POWERTRAY_H
#define POWERTRAY_H

#include <QtGui/QWidget>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QCoreApplication>
#include <QtDBus>
#include <QTimer>
#include <QFile>
#include <QTextStream>

class PowerTray : public QWidget
{
    Q_OBJECT

public:
    PowerTray(QWidget *parent = 0);
    ~PowerTray();

signals:
    void batteryCritical(int status);

private:
    QSystemTrayIcon *trayIcon;

private slots:
    void monitorBattery();
    void monitorLid();
    void batteryStatusCritical(int status);
};

#endif // POWERTRAY_H
