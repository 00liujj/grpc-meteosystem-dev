FORT += log no-doxygen pack no-crt-warn c++11 protoc
FORT_ROOT_REL = ../../../..

FORT_VER_PAT = 1
FORT_VER_DESCRIPTION = Программа "Определение метеопараметров в произвольной точке земного шара" 
FORT_VER_COPYRIGHT = 2017
DECIMAL = 11024

!include(../meteo-point_version.pri): error($${TARGET}": Failed to include ../meteo-point_version.pri")

#LIBS	+= -lspmutils -lsnsc -lgeocalc -lsynop-renderer -lcontour-points -ltriangulation -lpgnotify -lmeteo-client

INCLUDEPATH +=	$${FORT_ROOT_REL}/src/meteo-point/include

!include($${FORT_ROOT_REL}/src/common/fort.pri) {
	error("$$TARGET: File not found: fort.pri")
}

TARGET = meteo-point
TEMPLATE  = lib
CONFIG += qt
QT += network sql
CONFIG += dll

DEFINES += METEO_POINT_EXPORTS

INCLUDEPATH += 	../../../common/include \
		../../include \
		../include \
		../../../pg/include \

HEADERS += ../include/meteo-point.h \
			meteo-point_p.h \
			../../include/meteo-stations.h \
			../../include/meteo-source.h \
		
PROTOC  += ../../proto/synop-data.proto \
            ../../proto/temp-data.proto \
			../../proto/grib-data.proto 


SOURCES +=	meteo-point.cpp

win32:LIBS += -lws2_32 

#INSTALL_PATH = $$fort_packet(fort-extransport-client)
#target.path = $$INSTALL_PATH/bin
	   
AUTHORS = dmakarov
