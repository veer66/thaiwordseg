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
#include"wiwordcut.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>




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
  int *dellist;
  size_t delsize;
  int i, n_iter = 0, c = 0;
  unsigned char ch;
  size_t forest_size = len * 4;
  size_t forest_pos = len;
  WiIterator *iterator;
  MEMCHK (forest = (WiForest) malloc (sizeof (int) * forest_size));
  MEMCHK (iterator = (WiIterator *) malloc (sizeof (WiIterator) * len));
  MEMCHK (dellist = (int *)malloc(sizeof(int)*len));
  delsize=0;
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
#ifdef DEBUG_STR2FOREST
      printf ("***** CH=%c\tN_Iter=%d\n",ch,n_iter);
#endif
      for (i = 0; i < n_iter; i++)
	{
#ifdef DEBUG_STR2FOREST
	  printf ("Before-->ch=%c start,c,list,status=%d,%d,%d,%d\t", ch,
		  iterator[i].start, c, iterator[i].list,
		  iterator[i].status);
	  printf ("%s\n", copychr (str, iterator[i].start, c));
#endif 
	  transit (dict, ch, iterator[i].list, &(iterator[i].list),
		   &(iterator[i].status));
#ifdef DEBUG_STR2FOREST
	  printf ("After-->ch=%c start,c,list,status=%d,%d,%d,%d\t", ch,
		  iterator[i].start, c, iterator[i].list,
		  iterator[i].status);
	  printf ("%s\n", copychr (str, iterator[i].start, c));
#endif 
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
	      dellist[delsize++]=i;
	  
	    }
	
	}
      for(i=0;i<delsize;i++) 
	{
	  n_iter--;
	  if(n_iter!=i)
	    {
	      iterator[dellist[i]]=iterator[n_iter];
	    }
	}
      delsize=0;

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
  free(dellist);
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


static void 
dump_path(WiPath path,size_t len) 
{
  int i;
  for(i=0;i<=len;i++) 
    {
      printf ("%d ",path[i]);
    }
  printf ("\n");
}

static void
forest2path(WiForest forest,size_t len,WiPath *_path)
{
#define KNOWN_WEIGHT      1
#define UNKNOWN_WEIGHT    len
#define NIL_PATH          (-1)
  WiPath path;
  int *wtab;
  int last;
  MEMCHK(path=(WiPath)malloc(sizeof(int)*(len+1)));
  MEMCHK(wtab=(int *)malloc(sizeof(int)*(len+1)));
  wtab[0] = 0;
  for(last=0;last<len;last++)
    {
      int i;
      /* add unknown word 's path */
      wtab[last+1]=wtab[last]+UNKNOWN_WEIGHT;
      path[last+1]=last;
      /* add path for forest */
      for(i=forest[last];forest[i]!=WI_FOREST_TERMINATOR;i++) 
	{
	  int first=forest[i];
	  int w=wtab[first]+KNOWN_WEIGHT;
	  if (w<wtab[last+1])
	    {
	      wtab[last+1] = w;
	      path[last+1] = first;
	    }
	}
    }
  *_path=path;
  free(wtab);
}


static char*
path2str(const char* str,WiPath path,size_t len,const char *delim,size_t delim_len)
{

  int delim_count,i,begin,end,bufpos;
  size_t buflen;
  char* buffer;
  
  /* count delimiters */
  for(i=len,delim_count=0;i!=0;i=path[i],delim_count++) ;
  buflen=len+(delim_count-1)*delim_len;
  MEMCHK(buffer=(char*)malloc(sizeof(char)*(buflen+1)));
  buffer[buflen]='\0';
  bufpos=buflen;
  i=len;
  while (i!=0) 
    {
      int j,k;
      end=i-1;
      begin=path[i];
      if (begin!=0)
	{
	  bufpos=bufpos-end+begin-delim_len-1;
	  for(k=bufpos,j=0;j<delim_len;j++,k++) 
	    {
	      buffer[k]=delim[j];
	    }
	}
      else 
	{
	  bufpos=0;
	  k=0;
	}
      for(j=begin;j<=end;j++,k++) 
	{
	  buffer[k]=str[j]; 
	}
      i=begin;
    }
  return buffer;
}

void 
wi_wordcut_destroy(WiWordcut *self)
{
  free(self->dict);
}

void 
wi_wordcut_init(WiWordcut *self,const char *dictfile)
{
  self->dict=load_dict(dictfile);
}

WiWordcut* 
wi_wordcut_new(const char *dictfile)
{
  WiWordcut *self;
  MEMCHK(self=(WiWordcut*)malloc(sizeof(WiWordcut)));
  wi_wordcut_init(self,dictfile);
  
  return self;
}

void 
wi_wordcut_delete(WiWordcut *self)
{
  wi_wordcut_destroy(self);
  free(self);
}

WiResult*
wi_wordcut_cut(WiWordcut *self,const char* str,size_t len)
{
  WiResult *result;
  WiForest forest;
  WiPath path;
  MEMCHK(result=(WiResult*)malloc(sizeof(WiResult)));
  forest=str2forest(self->dict,str,len);
  forest2path(forest,len,&path);
  result->path=path;
  result->len=len;
  MEMCHK(result->str=strdup(str));
  free(forest);
  return result;
}

void
wi_result_delete(WiResult *self)
{
  free(self->str);
  free(self->path);
  free(self);
}

const char* 
wi_result_fetch_string(WiResult *result,const char* delim,size_t delim_len)
{
  return path2str(result->str
		  ,result->path
		  ,result->len
		  ,delim,delim_len);
}

typedef struct wi_string_array 
{
  char** elements;
  size_t size;
} WiStringArray;

WiStringArray*
wi_result_fetch_array(WiResult *result)
{
  WiStringArray *array;
  int token_count,i,array_i;
  size_t len=result->len;
  char* str=result->str;
  int* path=result->path;
  MEMCHK(array=(WiStringArray*)malloc(sizeof(WiStringArray)));
  /* count delimiters */
  i=len;
  token_count=0;
  while (i!=0) 
    {
      i=path[i];
      token_count++;
    }
  MEMCHK(array->elements=(char**)malloc(sizeof(char*)*token_count));
  i=len;
  array_i=token_count-1;
  while(i!=0)
    {
      int end,begin,j,k;
      char *p;
      end=i-1;
      begin=path[i];
      MEMCHK(p=(char*)malloc(sizeof(char)*(end-begin+2)));
      for(j=begin,k=0;j<=end;j++,k++)
	{
	  p[k]=str[j];
	}
      p[k]='\0';
      array->elements[array_i]=p;
    }
}
