#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# libsmf package version number, (as distinct from shared library version)
m4_define([libsmf_major], [1])
m4_define([libsmf_minor], [4])
m4_define([libsmf_version], [libsmf_major.libsmf_minor])

# libtool version related macros
# See: http://www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html
m4_define([libsmf_lt_current],  [1])
m4_define([libsmf_lt_revision], [0])
m4_define([libsmf_lt_age],      [1])

AC_PREREQ([2.69])
AC_INIT([libsmf],
        [libsmf_version],
        [http://bugs],
        [libsmf],
        [http://website])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([src/smf.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_SUBST([LIBSMF_MAJOR_VERSION],  [libsmf_major])
AC_SUBST([LIBSMF_MINOR_VERSION],  [libsmf_minor])
AC_SUBST([LIBSMF_API_VERSION],    [libsmf_lt_current.libsmf_lt_revision])
AC_SUBST([LIBSMF_VERSION],        [libsmf_version])

# libtool versioning
AC_SUBST([LT_CURRENT],  [libsmf_lt_current])
AC_SUBST([LT_REVISION], [libsmf_lt_revision])
AC_SUBST([LT_AGE],      [libsmf_lt_age])

# Checks for programs.
AC_PROG_CC
CFLAGS="$CFLAGS -W -Wall -Wno-sign-compare"  # that last one's a project for another day
LT_INIT

# Checks for libraries.
AC_CHECK_LIB([m], [pow])
AC_ARG_WITH([readline],
	    [AS_HELP_STRING([--with-readline],
	    [support fancy command line editing @<:@default=check@:>@])],
	    [],
	    [with_readline=check])

AS_IF([test "x$with_readline" != xno],
      [AC_CHECK_LIB([readline], [main],
		    [AC_SUBST([READLINE_LIBS], ["-lreadline -lncurses"])
		    AC_DEFINE([HAVE_LIBREADLINE], [1], [Define if you have libreadline])],
		    [if test "x$with_readline" != xcheck; then
		     AC_MSG_FAILURE(
				    [--with-readline was given, but test for readline failed])
		     fi
		     ], -lncurses)])


# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([memset pow strdup strerror strtol strchr])

# Check for GLib
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.2)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

# Check for GObject Introspection
GOBJECT_INTROSPECTION_CHECK([1.30.0])

case "$host" in
  *-cygwin|*-mingw*) WS2_32_IF_NEEDED=-lws2_32 ;;
esac
AC_SUBST([WS2_32_IF_NEEDED])

AC_CONFIG_FILES([Makefile
                 smf.pc
                 src/Makefile
                 man/Makefile
                 tests/Makefile])
AC_OUTPUT
