#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT(TUIFramework, 0.16)
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([tuiframework/client/TUIClientAppProvider.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

######################################################################
# DOXYGEN SUPPORT
######################################################################

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([TUIFramework],[doxygen.cfg],[doc])

######################################################################
AC_MSG_NOTICE([Xerces C++ library configuration])
AX_LIB_XERCES([3.1.1])
if test "x$HAVE_XERCES" != "xyes"; then
   AC_MSG_ERROR([cannot find the Apache Xerces library])
fi
######################################################################
AC_MSG_NOTICE([pthreads configuration])
AX_PTHREAD([HAVE_PTHREADS="yes"],[HAVE_PTHREADS="no"])
if test "x$HAVE_PTHREADS" != "xyes"; then
   AC_MSG_ERROR([cannot find pthreads])
fi
######################################################################
AC_MSG_NOTICE([phidgets configuration])
AC_CHECK_LIB([phidget21], [CPhidgetInterfaceKit_create], [FOUND_PHIDGET21=yes], [FOUND_PHIDGET21=no])
AM_CONDITIONAL([PHIDGET21_AVAILABLE], [test "x$FOUND_PHIDGET21" = "xyes"])
######################################################################
AC_CHECK_LIB(c, dlopen, LIBDL="", [AC_CHECK_LIB(dl, dlopen, LIBDL="-ldl")])
AC_SUBST(LIBDL)
######################################################################

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
#AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([memset select socket])


AC_CONFIG_FILES([Makefile
                 tuiframework/Makefile
                 tuilibs/Makefile
                 tuibins/Makefile])
AC_OUTPUT

