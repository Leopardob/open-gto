#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(gto_maya_location)/include

AM_CXXFLAGS = -fno-exceptions -D_BOOL -D__USER__=\"$$USER\"

plugindir = $(gto_maya_location)/bin/plug-ins
meldir = $(gto_maya_location)/scripts/others

if GTO_BUILD_MAYA

dist_mel_SCRIPTS = gtoExportOptions.mel

plugin_PROGRAMS = GtoIO.so

GtoIO_so_SOURCES = GtoInDataBase.cpp \
                   GtoInNURBS.cpp \
                   GtoInObject.cpp \
                   GtoInParticle.cpp \
                   GtoInPoly.cpp \
                   GtoInReader.cpp \
                   GtoInSet.cpp \
                   GtoInTransform.cpp \
                   GtoIO.cpp \
                   GtoOut.cpp \
                   version.cpp \
                   GtoMayaAttribute.cpp \
                   GtoAnimCurve.cpp \
                   TwkMaya.cpp

noinst_HEADERS = GtoInDataBase.h \
                 GtoInNURBS.h \
                 GtoInObject.h \
                 GtoInParticle.h \
                 GtoInPoly.h \
                 GtoInReader.h \
                 GtoInSet.h \
                 GtoInTransform.h \
                 GtoIO.h \
                 GtoOut.h \
                 version.h \
                 GtoMayaAttribute.h \
                 GtoAnimCurve.h \
                 TwkMaya.h

GtoIO_so_LDFLAGS = -shared \
                   -L$(top_srcdir)/lib/Gto \
                   -L$(gto_maya_location)/lib \
                   -lGto \
                   -lImage \
                   -lOpenMaya \
                   -lOpenMayaAnim \
                   -lOpenMayaRender 

endif