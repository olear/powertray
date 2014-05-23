/*
# PowerTray - Manage Power States on Dracolinux
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
#
# PowerTray is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 2.
*/

#include "powertray.h"

PowerTray::PowerTray(QWidget *parent)
    : QWidget(parent)
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/res/tray.png"));
    trayIcon->setToolTip("PowerTray");
    connect(this,SIGNAL(batteryCritical(int)),this,SLOT(batteryStatusCritical(int)));
    QTimer::singleShot(0,this,SLOT(monitorBattery()));
    QTimer::singleShot(0,this,SLOT(monitorLid()));
}

PowerTray::~PowerTray()
{
    delete trayIcon;
}

void PowerTray::monitorLid()
{
    bool do_suspend = false;
    QStringList lids;
    lids << "/proc/acpi/button/lid/LID/state" << "/proc/acpi/button/lid/LID0/state";
    for (int i = 0; i < lids.size(); ++i)
    {
        QFile lid(lids.at(i));
        if (lid.exists())
        {
            if (lid.open(QIODevice::ReadOnly))
            {
                QString line;
                QTextStream textStream(&lid);
                line = textStream.readAll().simplified();
                if (line == "state: closed")
                {
                    do_suspend = true;
                }
                lid.close();
            }
        }
    }

    if (do_suspend)
    {
        QDBusConnection conn = QDBusConnection::systemBus();
        QDBusInterface computer("org.freedesktop.Hal","/org/freedesktop/Hal/devices/computer","org.freedesktop.Hal.Device.SystemPowerManagement",conn);
        computer.call("Suspend",1);
    }

    QTimer::singleShot(1000,this,SLOT(monitorLid())); // loop
}

void PowerTray::monitorBattery()
{
    QDBusConnection conn = QDBusConnection::systemBus();
    QDBusInterface hal("org.freedesktop.Hal","/org/freedesktop/Hal/Manager","org.freedesktop.Hal.Manager",conn);
    QDBusMessage msg = hal.call("FindDeviceByCapability", "battery");
    QList<QVariant> devices = msg.arguments();

    int battery_level = 0;
    bool battery_active = false;
    bool battery_exists = false;
    foreach (QVariant name, devices)
    {
        QString bat = name.toStringList()[0];
        QDBusInterface device("org.freedesktop.Hal",bat,"org.freedesktop.Hal.Device",conn);
        QDBusReply<bool> battery_present = device.call("GetPropertyBoolean","battery.present");
        if (battery_present)
        {
            battery_exists = true;
            QDBusReply<int> battery_charge_level = device.call("GetPropertyInteger","battery.charge_level.percentage");
            QDBusReply<bool> battery_discharging = device.call("GetPropertyBoolean","battery.rechargeable.is_discharging");
            if (battery_discharging)
            {
                battery_active = true;
            }
            else
            {
                battery_active = false;
            }
            battery_level = battery_level + battery_charge_level;
        }
    }
    if (battery_exists)
    {
        if (!trayIcon->isVisible())
        {
            trayIcon->show();
        }
    }
    else
    {
        if (trayIcon->isVisible())
        {
            trayIcon->hide();
        }
    }

    if (battery_level>0)
    {
        trayIcon->setToolTip(tr("Battery")+" "+QString::number(battery_level)+"%");
        trayIcon->setIcon(QIcon(":/res/tray.png"));
        if (battery_active)
        {
            if (battery_level<=10) // Battery level is Low, notify user
            {
                trayIcon->setIcon(QIcon(":/res/tray-warn.png"));
            }
            if (battery_level<=5) // Battery is critical, suspend/shutdown computer
            {
                emit batteryCritical(battery_level);
            }
        }
    }

    QTimer::singleShot(30000,this,SLOT(monitorBattery()));
}

void PowerTray::batteryStatusCritical(int status)
{
    if (status>=3) // Suspend computer when level is above 3%
    {
        QDBusConnection conn = QDBusConnection::systemBus();
        QDBusInterface computer("org.freedesktop.Hal","/org/freedesktop/Hal/devices/computer","org.freedesktop.Hal.Device.SystemPowerManagement",conn);
        computer.call("Suspend",1);
    }
    else // Shutdown computer if level is under 3%
    {
        QDBusConnection conn = QDBusConnection::systemBus();
        QDBusInterface computer("org.freedesktop.Hal","/org/freedesktop/Hal/devices/computer","org.freedesktop.Hal.Device.SystemPowerManagement",conn);
        computer.call("Shutdown",1);
    }
}
