dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/bigrams.c)
AM_INIT_AUTOMAKE(bkidxgen,0.0.1pre1)
AC_SUBST(VERSION)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_PKG_FIND(GLIB2,glib-2.0)
AC_SUBST(GLIB2_CFLAGS)
AC_SUBST(GLIB2_LIBS)

AC_PKG_FIND(WORDCUT,wordcut)
AC_SUBST(WORDCUT_CFLAGS)
AC_SUBST(WORDCUT_LIBS)

AC_PKG_FIND(LATEXPARSER,latexparser)
AC_SUBST(LATEXPARSER_CFLAGS)
AC_SUBST(LATEXPARSER_LIBS)



dnl Checks for header files.
#AC_STDC_HEADERS
AC_HAVE_HEADERS(string.h)

dnl Checks for typedefs, structures, and compiler characteristics.
#AC_C_CONST

dnl Checks for library functions.
#AC_FUNC_ALLOCA
AC_OUTPUT(	
	Makefile 
	src/Makefile 
)
