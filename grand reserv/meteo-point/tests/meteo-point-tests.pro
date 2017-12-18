FORT += log apr no-crt-warn cppunit-noauto no-doxygen no-version tests c++11
FORT_ROOT_REL = ../../../../..

TEMPLATE = app
DEFINES += UNICODE

!include($${FORT_ROOT_REL}/src/common/fort.pri) {
	error($${TARGET}": File not found: fort.pri")
}

CONFIG += qt
CONFIG += console

INCLUDEPATH +=  ../../include \


HEADERS += \



SOURCES += meteo-point-tests.cpp		

AUTHORS = dmakarov
