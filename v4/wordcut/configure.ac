dnl Process this file with autoconf to produce a configure script.
AC_INIT(wordcut/wordcut_mkdict.c)
AM_INIT_AUTOMAKE(wordcut,cvs`date +%Y%m%d`)
AC_SUBST(VERSION)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL


dnl Checks for header files.
#AC_STDC_HEADERS
#AC_HAVE_HEADERS(string.h)

dnl Checks for typedefs, structures, and compiler characteristics.
#AC_C_CONST

dnl Checks for library functions.
#AC_FUNC_ALLOCA
AC_OUTPUT(	
	Makefile  \
	wordcut/Makefile \
	data/Makefile \
	wordcut/path.h \
	wordcut.pc
)
