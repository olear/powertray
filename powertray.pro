#
# PowerTray - Manage Power States on Dracolinux
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
#
# PowerTray is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 2.
#

VERSION                              = 20140523
QT                                  += core gui dbus
TARGET                               = powertray
TEMPLATE                             = app

SOURCES                             += src/main.cpp src/powertray.cpp
HEADERS                             += src/powertray.h
RESOURCES                           += res/res.qrc

DESTDIR = build
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc

isEmpty(PREFIX) {
    PREFIX                           = /usr/local
}

isEmpty(DOCDIR) {
    DOCDIR                           = $$PREFIX/share/doc
}

target.path                          = $${PREFIX}/bin
target_docs.path                     = $$DOCDIR/$${TARGET}-$${VERSION}
target_docs.files                    = doc/COPYING doc/README

INSTALLS                            += target target_docs
QMAKE_CLEAN                         += -r $${DESTDIR} Makefile
#QMAKE_POST_LINK                     += $$quote(strip -s $${DESTDIR}/$${TARGET}$$escape_expand(\\n\\t))

DEFINES                             += APPV=\"\\\"$${VERSION}\\\"\"

message("PREFIX: $$PREFIX")
message("DOCDIR: $${DOCDIR}")
