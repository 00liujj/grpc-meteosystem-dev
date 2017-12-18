FORT += log service odf no-doxygen pack no-crt-warn c++11
FORT_ROOT_REL = ../../../..

FORT_VER_PAT = 1
FORT_VER_DESCRIPTION = Программа "Тесты для метео в точке" 
FORT_VER_COPYRIGHT = 2016
DECIMAL = 11024

!include(../meteo-point_version.pri): error($${TARGET}": Failed to include ../meteo-point_version.pri")

LIBS	+= -lserviceskel -ldl -lmeteo-point

INCLUDEPATH +=	$${FORT_ROOT_REL}/src/meteo-point/include

!include($${FORT_ROOT_REL}/src/common/fort.pri) {
	error("$$TARGET: File not found: fort.pri")
}
DEFINES += METEO_POINT_EXPORTS

TARGET = meteo-point-tester
CONFIG += qt
QT += sql core

INCLUDEPATH += 	../../../common/include \
		../../include \
		../include \
		../../../pg/include \

HEADERS +=      ../include/meteo-point.h \


SOURCES +=	meteo-point-tester.cpp
			
ODFS += meteo-point-tester.odf

#win32:LIBS += -lws2_32 

#INSTALL_PATH = $$fort_packet(fort-extransport-client)
#target.path = $$INSTALL_PATH/bin
	   
AUTHORS = dmakarov
