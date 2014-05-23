/*
# PowerTray - Manage Power States on Dracolinux
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
#
# PowerTray is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 2.
*/

#include <QtGui/QApplication>
#include "powertray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setApplicationName("PowerTray");
    QApplication::setApplicationVersion(APPV);
    QApplication::setOrganizationName("DracoLinux");
    QApplication::setOrganizationDomain("dracolinux.org");
    PowerTray w;

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return 1;
    }

    return a.exec();
}
