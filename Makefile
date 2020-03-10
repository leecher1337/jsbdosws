#
# (C) Copyright 1992, ..., 2007 the "DOSEMU-Development-Team".
#
# for details see file COPYING.DOSEMU in the DOSEMU distribution
#

top_builddir=../../..
include $(top_builddir)/Makefile.conf

D:=$(BINPATH)/commands

CFILES = jsbdosws.c mod_dosemu.c
HFILES = jsbdosws.h vslfunc_linux.h
EXE    = $(D)/jsbdosws.exe
DEPENDS= $(CFILES:.c=.d)
OBJS   = $(CFILES:.c=.o)

ALL = $(CFILES) $(HFILES)

$(D)/%.exe : dev/dosemu/%.exe
	cp $< $@

all: lib $(SYS)

install:

include $(REALTOPDIR)/src/Makefile.common
