AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

$(shell ./scripts/pics.sh)

bin_PROGRAMS = FreeMine

FreeMine_SOURCES = main.c

FreeMine_CPPFLAGS = $(CFLAGS) -Wall -Wextra -std=c90
FreeMine_LDADD = libmainmenu.la
FreeMine_LDFLAGS = $(LDFLAGS)

include_HEADERS = headers/mines.h

lib_LTLIBRARIES = libfreemine_main.la

# libfreemine_main.la
libfreemine_main_la_LDFLAGS = -version-info 0:0:0
libfreemine_main_la_SOURCES = libs/help_routines.c libs/draw.c libs/mines.c libs/menu.c libs/lead.c
libfreemine_main_la_CFLAGS = $(FreeMine_CFLAGS)