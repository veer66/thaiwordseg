TESTS=test_pos test_wordunit test_dictmap
CFLAGS = -I$(top_builddir)/include
check_PROGRAMS=test_pos test_wordunit test_dictmap
test_pos_SOURCES=test_pos.c
test_pos_CFLAGS=`pkg-config glib-2.0 --cflags`
test_pos_LDFLAG=`pkg-config glib-2.0 --libs`
test_pos_LDADD=$(top_builddir)/src/libwordcut.la

test_wordunit_SOURCES=test_wordunit.c
test_wordunit_CFLAGS=`pkg-config glib-2.0 --cflags`
test_wordunit_LDFLAG=`pkg-config glib-2.0 --libs`
test_wordunit_LDADD=$(top_builddir)/src/libwordcut.la

test_dictmap_SOURCES=test_dictmap.c
test_dictmap_CFLAGS=`pkg-config glib-2.0 --cflags`
test_dictmap_LDFLAG=`pkg-config glib-2.0 --libs`
test_dictmap_LDADD=$(top_builddir)/src/libwordcut.la
