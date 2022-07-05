# Autoconf requirements

# Initialize, but use more options. Note parameters brackts and whitespace.
AC_INIT([FreeMine], [1.0], [anton2920@gmail.com])

# Safety check - list a source file that wouldn't be in other directories:
AC_CONFIG_SRCDIR([src/main.c])

# Put configuration results here, so we can easily #include them:
AC_CONFIG_HEADERS([src/headers/FreeMine_config.h])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Enable «automake» to simplify creating makefiles:
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile src/Makefile])

# Look for a C (CXX) compiler:
AC_LANG([C])
AC_PROG_CC

# Check for: (more) programs, libraries, header files, types,
# structures, compiler characteristics, library functions, system services.
LT_INIT

AC_ENABLE_SHARED
AC_PROG_LIBTOOL

# Check for SDL2
AC_SEARCH_LIBS([SDL_Init], [SDL2], [], [
	AC_MSG_ERROR([SDL2 is required to compile and run this game])
	])

AC_SEARCH_LIBS([IMG_Load], [SDL2_image], [], [
	AC_MSG_ERROR([SDL2_image is required to compile and run this game])
	])

AC_SEARCH_LIBS([TTF_Init], [SDL2_ttf], [], [
	AC_MSG_ERROR([SDL2_ttf is required to compile and run this game])
	])

AC_SEARCH_LIBS([Mix_Init], [SDL2_mixer], [], [
	AC_MSG_ERROR([SDL2_mixer is required to compile and run this game])
	])

AC_CHECK_HEADERS([SDL2/SDL.h], [], [])

AC_CHECK_HEADERS([SDL/SDL.h], [], [],
[#ifndef HAVE_SDL2_SDL_H && HAVE_SDL_H
#include <SDL/SDL.h>
endif
])

AC_CHECK_HEADERS([SDL.h], [], [],
[#ifndef HAVE_SDL2_SDL_H && HAVE_SDL_SDL_H
#include <SDL.h>
#endif
])

# Check for essential libraries 
AC_SEARCH_LIBS([printf], [c], [], [
	AC_MSG_ERROR([unable to find c lib])
	])

AC_CHECK_HEADERS([stdio.h stdlib.h assert.h string.h time.h], [], [
	AC_MSG_ERROR([unable to find necessary headers])
	])

 # Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Do final output.
AC_OUTPUT

echo 
echo " ---------------------------------"
echo "| Options used to compile and link:"
echo "| CC            = $CC"
echo "| CFLAGS        = $CFLAGS"
echo "| LDFLAGS       = $LDFLAGS"
echo "| ARFLAGS       = $ARFLAGS"
echo " ---------------------------------"