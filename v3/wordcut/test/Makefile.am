INCLUDES= -I$(top_srcdir)/include
TESTS=test_pos test_wordunit test_dictmap test_wordcut

check_PROGRAMS=test_pos test_wordunit test_dictmap test_wordcut
test_pos_SOURCES=test_pos.c
test_pos_LDADD=$(top_builddir)/src/libwordcut.la

test_wordunit_SOURCES=test_wordunit.c
test_wordunit_LDADD=$(top_builddir)/src/libwordcut.la

test_dictmap_SOURCES=test_dictmap.c
test_dictmap_LDADD=$(top_builddir)/src/libwordcut.la

test_wordcut_SOURCES=test_wordcut.c
test_wordcut_LDADD=$(top_builddir)/src/libwordcut.la

