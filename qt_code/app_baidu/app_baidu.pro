#-------------------------------------------------
#
# Project created 2012-09-11
#
#-------------------------------------------------

QT += core gui
QT += network
QT += webkit

HEADERS += lg_packet.h

HEADERS += network.h
HEADERS += keyword.h

HEADERS += task_table.h
HEADERS += task_insert.h
HEADERS += task_update.h
HEADERS += task_delete.h

HEADERS += login_window.h
HEADERS += chat_window.h
HEADERS += main_window.h


SOURCES += network.cpp
SOURCES += keyword.cpp

SOURCES += task_table.cpp
SOURCES += task_insert.cpp
SOURCES += task_update.cpp
SOURCES += task_delete.cpp

SOURCES += login_window.cpp
SOURCES += chat_window.cpp
SOURCES += main_window.cpp

SOURCES += main.cpp


RESOURCES += images.qrc


