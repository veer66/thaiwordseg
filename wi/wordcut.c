/*
 * wordcut.c - Wi word segmentation for Thai Language.
 *
 * Wi
 * Copyright (C) 2002 Vee Satayamas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define MEMCHK(mem) if( (mem) == NULL ) { \
  fprintf(stderr,"%s:%d: Out of memory.\n",__FILE__,__LINE__); exit(1); }

typedef unsigned int *widict_t;
typedef int wi_boolean;

#define WI_FALSE 	0
#define WI_TRUE		1

typedef struct wiwordcut_t
{
  widict_t dicttab;
}
WiWordcut;

typedef int *WiForest;
typedef int *WiPath;



#define SIZE_MASK(x)			((x) & 0x7FFFFFFF)
#define IS_TABLE_MASK(x)		((x) & 0x80000000)
#define CHAR_MASK(x)			((x) & 0xFF000000)
#define EXT_CHAR(x)             (((x) & 0xFF000000)>>24)
#define TERMINATOR_MASK(x)		((x) & 0x00800000)
#define POINT_MASK(x)			((x) & 0x007FFFFF)
#define RECORD_SIZE				4

#define WI_FOREST_TERMINATOR	(-1)
static widict_t
load_dict (const char *filename)
{
/* #define USE_FREAD */
#ifndef USE_FREAD
  int fd;
  size_t n;
  int count, pos;
  widict_t dicttab;

  fd = open (filename, O_RDONLY);
  if (fd == -1)
    {
      fprintf (stderr, "can't open dictionary file (%s)\n", filename);
      exit (1);
    }
  if (read (fd, &n, RECORD_SIZE) != RECORD_SIZE)
    {
      fprintf (stderr, "Error reading dictionary header (%s)\n", filename);
      exit (1);
    }
  n = (n - 1) * RECORD_SIZE;
  MEMCHK (dicttab = (widict_t) malloc (RECORD_SIZE * n));
  pos = 0;
  while (n - pos > 0)
    {

      count = read (fd, dicttab + pos, n - pos);
      if (count == -1)
	{
	  fprintf (stderr, "Error reading dictionary (%s)\n", filename);
	  exit (1);
	}
      else
	{
	  pos += count;
	}
    }
  close (fd);
#else
  FILE *fp;
  size_t n;
  widict_t dicttab;

  fp = fopen (filename, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "can't open dictionary file (%s)\n", filename);
      exit (1);
    }
  fread (&n, RECORD_SIZE, 1, fp);
  n--;
  if (n < 0)
    {
      fprintf (stderr, "invalid dictionary (%s)\n", filename);
      exit (1);
    }
  MEMCHK (dicttab = (widict_t) malloc (RECORD_SIZE * n));
  fread (dicttab, RECORD_SIZE, n, fp);
  fclose (fp);
#endif
  return dicttab;
}

#define HEADER_SIZE			1	/* 1 record = four bytes */
#define DEAD				0
#define ACTIVE				1
#define ACTIVE_TERMINATE 	2

void
transit (widict_t dicttab, unsigned char ch, int pos, int *next, int *status)
{
  /* 
   * It is very critical to give next to the save varible as pos. 
   * Becareful to modify. 
   */
  if (pos == 0)
    {
      *status = DEAD;
      return;
    }
  pos--;
  unsigned int data = dicttab[pos];

  if (IS_TABLE_MASK (data) != 0)
    {
      /* Table */
      unsigned int p;
      int t = pos + ch + HEADER_SIZE;

      if (CHAR_MASK ((p = dicttab[t])) != 0)
	{
	  /* char is exist */
	  if (TERMINATOR_MASK (p) != 0)
	    {
	      *status = ACTIVE_TERMINATE;
	    }
	  else
	    {
	      *status = ACTIVE;
	    }
	  *next = POINT_MASK (dicttab[t]);
	}
      else
	{
	  *status = DEAD;
	}
    }
  else
    {
      size_t size = SIZE_MASK (data);
      size_t l = 0, r = size - 1;
      size_t m;
      unsigned char tch;
      wi_boolean found = WI_FALSE;

      while (l <= r && l >= 0 && r < size)
	{
	  m = ((l + r) >> 1);
	  tch = EXT_CHAR (dicttab[pos + m + HEADER_SIZE]);
	  assert (l >= 0 && r < size);
	  if (ch < tch)
	    {
	      r = m - 1;
	    }
	  else if (ch > tch)
	    {
	      l = m + 1;
	    }
	  else
	    {
	      found = WI_TRUE;
	      break;
	    }
	}
      if (found)
	{
	  unsigned int p = dicttab[pos + m + HEADER_SIZE];

	  if (TERMINATOR_MASK (p) != 0)
	    {
	      *status = ACTIVE_TERMINATE;
	    }
	  else
	    {
	      *status = ACTIVE;
	    }
	  *next = POINT_MASK (p);
	}
      else
	{
	  *status = DEAD;
	}
    }
}

typedef struct wi_iterator_t
{
  int status, list, start;
}
WiIterator;

static const char *
copychr (const char *str, int begin, int end)
{
#define MAX_BUFFER 1024
  int i;
  static char buffer[MAX_BUFFER];
  int c = 0;

  assert (end - begin + 1 < MAX_BUFFER);
  for (i = begin; i <= end; i++)
    {
      buffer[c++] = str[i];
    }
  buffer[c] = '\0';
  return buffer;
}

static void
forest_show(WiForest forest,const char *str,size_t len)
{
  int i,j;
  printf ("TAB = ");
  for(i=0;i<len;i++) 
    {
      printf ("%d ",forest[i]);
    }
  printf ("\n");
  for(i=0;i<len;i++) 
    {
      printf ("I=%d\t",i);
      for(j=forest[i];forest[j]!=(-1);j++) 
	{
	  printf ("%d,%s ",forest[j],copychr(str,forest[j],i));
	}
      printf ("\n");
    }
  printf ("\n");
}

static void 
forest_dump(WiForest forest,size_t size)
{
  int i;
  for(i=0;i<size;i++)
    {
      printf ("%d,%d ",i,forest[i]);
    }
  printf ("\n");
}

static WiForest
str2forest (widict_t dict, const unsigned char *str, size_t len)
{
#define STARTPOS 	1


  const unsigned char *p;
  WiForest forest;
  int i, n_iter = 0, c = 0;
  unsigned char ch;
  unsigned int temp;
  size_t map_count = 0;
  size_t forest_size = len * 4;
  size_t forest_pos = len;
  WiIterator *iterator;
  MEMCHK (forest = (WiForest) malloc (sizeof (int) * forest_size));
  MEMCHK (iterator = (WiIterator *) malloc (sizeof (WiIterator) * len));
  for(p = str; *p != '\0'; p++, c++)
    {
      ch = *p;
      /* start forest */
      forest[c] = forest_pos;

      /* add new list item */
      iterator[n_iter].start = c;
      iterator[n_iter].list = STARTPOS;
      iterator[n_iter].status = ACTIVE;
      n_iter++;
      /* check */
      forest[0] = len;
      for (i = 0; i < n_iter; i++)
	{

	  transit (dict, ch, iterator[i].list, &(iterator[i].list),
		   &(iterator[i].status));
	  if (iterator[i].status != DEAD)
	    {
	      if (iterator[i].status == ACTIVE_TERMINATE)
		{
		  /* add node to forest */
		  forest[forest_pos] = iterator[i].start;
		  forest_pos++;
		  if (forest_pos >= forest_size)
		    {
		      forest_size = forest_size + forest_size;
		      MEMCHK (forest =
			      (WiForest) realloc (forest,
						  forest_size));
		    }

#ifdef DEBUG_STR2FOREST
		  forest_dump(forest,forest_pos);
		  printf ("-->ch=%c start,c,list,status=%d,%d,%d,%d\t", ch,
			  iterator[i].start, c, iterator[i].list,
			  iterator[i].status);
		  printf ("%s\n", copychr (str, iterator[i].start, c));
#endif

		}
	    }
	  else
	    {
	      /* delete */
	      n_iter--;
	      if (n_iter != i)
		{
		  /* move */
		  iterator[i] = iterator[n_iter];
		}
	    }
	
	}
      /* terminate */
      forest[forest_pos] = WI_FOREST_TERMINATOR;
      forest_pos++;
      if (forest_pos >= forest_size)
	{
	  forest_size = forest_size + forest_size;
	  MEMCHK (forest =
		  (WiForest) realloc (forest, forest_size));
	}   
    }

  free (iterator);
  return forest;
}

void
test_transit ()
{
  widict_t dict;
  unsigned int next, status, pos;
  unsigned char ch;
  char *str = "ตากลม";

  dict = load_dict ("test5.out");
  pos = 1;
  do
    {
      ch = *(str++);
      printf ("!!!1\n");
      transit (dict, ch, pos, &next, &status);
      printf ("!!!2\n");
      printf ("%c\tnext=%d,status=%d\n", ch, next, status);
      pos = next;
    }
  while (status != DEAD && next != 0);
  free (dict);
}

WiPath
forest2path(WiForest forest,size_t len)
{
  WiPath path;
  int i;
  MEMCHK(path=(WiPath)malloc(sizeof(int)*len));
  for(i=0;i<len;i++)
    {
      
    }
}

int
main (int argc, char **argv)
{
  widict_t dict;
  char *str = "ตากลม";
  WiForest forest;
  dict = load_dict ("test5.out");
  forest=str2forest (dict, str, 5);
  forest_show(forest,"ตากลม",5);
  free (dict);
  return 0;
}
