AC_DEFUN([AC_OLD_PKG_FIND],[
    AC_PATH_PROG(OLD_PKG_CFG,$2-config,no)
    if test "$OLD_PKG_CFG" = "no"; then
        AC_MSG_ERROR([ $2-config is not found.])
    else
        AC_MSG_CHECKING($1_CFLAGS)
        $1_CFLAGS=`$2-config --cflags`
        AC_MSG_RESULT($$1_CFLAGS)

        AC_MSG_CHECKING($1_LIBS)
        $1_LIBS=`$2-config --libs`
        AC_MSG_RESULT($$1_LIBS)
    fi
])

AC_DEFUN([AC_PKG_FIND],[
    if test -z "$PKG_CONFIG"; then
        AC_PATH_PROG(PKG_CONFIG,pkg-config,no)
    fi
    if test "$PKG_CONFIG" = "no"; then
        AC_MSG_ERROR([pkg-config is not found.])
    else
        AC_MSG_CHECKING(for $2)
        if `$PKG_CONFIG --exists $2`; then 
            AC_MSG_RESULT(yes)
            
            AC_MSG_CHECKING($1_CFLAGS)
            $1_CFLAGS=`$PKG_CONFIG --cflags "$2"`
            AC_MSG_RESULT($$1_CFLAGS)

            AC_MSG_CHECKING($1_LIBS)
            $1_LIBS=`$PKG_CONFIG --libs "$2"`
            AC_MSG_RESULT($$1_LIBS)
        else
            AC_MSG_ERROR([Package $2 is not found.])
        fi
    fi
])


