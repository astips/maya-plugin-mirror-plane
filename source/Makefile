#-
# ==========================================================================
# Copyright (c) 2011 Autodesk, Inc.
# All rights reserved.
# 
# These coded instructions, statements, and computer programs contain
# unpublished proprietary information written by Autodesk, Inc., and are
# protected by Federal copyright law. They may not be disclosed to third
# parties or copied or duplicated in any form, in whole or in part, without
# the prior written consent of Autodesk, Inc.
# ==========================================================================
#+

ifndef INCL_BUILDRULES

#
# Include platform specific build settings
#
TOP := ..
include $(TOP)/buildrules


#
# Always build the local plug-in when make is invoked from the
# directory.
#
all : plugins

endif

#
# Variable definitions
#

PLUGIN_NAME := mirrorPlane

SRCDIR := $(TOP)/mirrorPlane
DSTDIR := $(TOP)/mirrorPlane/build

$(PLUGIN_NAME)_SOURCES  := $(TOP)/mirrorPlane/mirrorPlane.cpp
$(PLUGIN_NAME)_OBJECTS  := $(TOP)/mirrorPlane/mirrorPlane.o
$(PLUGIN_NAME)_PLUGIN   := $(DSTDIR)/$(PLUGIN_NAME).$(EXT)
$(PLUGIN_NAME)_MAKEFILE := $(DSTDIR)/Makefile

#
# Include the optional per-plugin Makefile.inc
#
#    The file can contain macro definitions such as:
#       {pluginName}_EXTRA_CFLAGS
#       {pluginName}_EXTRA_C++FLAGS
#       {pluginName}_EXTRA_INCLUDES
#       {pluginName}_EXTRA_LIBS
-include $(SRCDIR)/Makefile.inc

#
# Set target specific flags.
#

$($(PLUGIN_NAME)_OBJECTS): CFLAGS   := $(CFLAGS)   $($(PLUGIN_NAME)_EXTRA_CFLAGS)
$($(PLUGIN_NAME)_OBJECTS): C++FLAGS := $(C++FLAGS) $($(PLUGIN_NAME)_EXTRA_C++FLAGS) -DNDEBUG -msse3
$($(PLUGIN_NAME)_OBJECTS): INCLUDES := $(INCLUDES) $($(PLUGIN_NAME)_EXTRA_INCLUDES) -I../nlib -I. -I$(MAYA_LOCATION)/include -I/usr/include/X11/

depend_$(PLUGIN_NAME):     INCLUDES := $(INCLUDES) $($(PLUGIN_NAME)_EXTRA_INCLUDES)

$($(PLUGIN_NAME)_PLUGIN):  LFLAGS   := $(LFLAGS) $($(PLUGIN_NAME)_EXTRA_LFLAGS) 
$($(PLUGIN_NAME)_PLUGIN):  LIBS     := $(LIBS)   -lOpenMaya -lOpenMayaAnim -lOpenMayaUI -lOpenMayaRender -lFoundation -lclew $($(PLUGIN_NAME)_EXTRA_LIBS)

# -lOpenGL
#
# Rules definitions
#

.PHONY: depend_$(PLUGIN_NAME) clean_$(PLUGIN_NAME) Clean_$(PLUGIN_NAME)


$($(PLUGIN_NAME)_PLUGIN): $($(PLUGIN_NAME)_OBJECTS) 
	-rm -f $@
	$(LD) -o $@ $(LFLAGS) $^ $(LIBS)

depend_$(PLUGIN_NAME) :
	makedepend $(INCLUDES) $(MDFLAGS) -f$(DSTDIR)/Makefile $($(PLUGIN_NAME)_SOURCES)

clean_$(PLUGIN_NAME):
	-rm -f $($(PLUGIN_NAME)_OBJECTS)

Clean_$(PLUGIN_NAME):
	-rm -f $($(PLUGIN_NAME)_MAKEFILE).bak $($(PLUGIN_NAME)_OBJECTS) $($(PLUGIN_NAME)_PLUGIN)


plugins: $($(PLUGIN_NAME)_PLUGIN)
depend:	 depend_$(PLUGIN_NAME)
clean:	 clean_$(PLUGIN_NAME)
Clean:	 Clean_$(PLUGIN_NAME)
