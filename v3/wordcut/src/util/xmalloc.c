/* xmalloc.c -- malloc with out of memory checking
   Copyright (C) 1990, 91, 92, 93, 94, 95, 96, 99 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif



#include <sys/types.h>
#include <wordcut/wccommon.h>
#if STDC_HEADERS
# include <stdlib.h>
#else
VOID *calloc ();
VOID *malloc ();
VOID *realloc ();
void free ();
#endif

#if ENABLE_NLS
# include <libintl.h>
# define _(Text) gettext (Text)
#else
# define textdomain(Domain)
# define _(Text) Text
#endif

#include "error.h"

#ifndef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

/* Prototypes for functions defined here.  */
#if defined (__STDC__) && __STDC__
static VOID *fixup_null_alloc (size_t n);
#endif


/* Exit value when the requested amount of memory is not available.
   The caller may set it to some other value.  */
int xmalloc_exit_failure = EXIT_FAILURE;

static VOID *
fixup_null_alloc (n)
     size_t n;
{
  VOID *p;

  p = 0;
  if (n == 0)
    p = malloc ((size_t) 1);
  if (p == 0)
    WC_HALT("Out of memory.");
  return p;
}

/* Allocate N bytes of memory dynamically, with error checking.  */

VOID *
wc_xmalloc (n)
     size_t n;
{
  VOID *p;

  p = malloc (n);
  if (p == 0)
    p = fixup_null_alloc (n);
  return p;
}

/* Allocate memory for N elements of S bytes, with error checking.  */

VOID *
wc_xcalloc (n, s)
     size_t n, s;
{
  VOID *p;

  p = calloc (n, s);
  if (p == 0)
    p = fixup_null_alloc (n);
  return p;
}

/* Change the size of an allocated block of memory P to N bytes,
   with error checking.
   If P is NULL, run xmalloc.  */

VOID *
wc_xrealloc (p, n)
     VOID *p;
     size_t n;
{
  if (p == 0)
    return wc_xmalloc (n);
  p = realloc (p, n);
  if (p == 0)
    p = fixup_null_alloc (n);
  return p;
}

/* Make a copy of a string in a newly allocated block of memory. */

char *
wc_xstrdup (char *str)
{
  VOID *p;

  p = wc_xmalloc (strlen (str) + 1);
  strcpy (p, str);
  return p;
}
