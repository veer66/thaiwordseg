#ifndef __WI_COMMON_H__
#define __WI_COMMON_H__
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define PACKAGE       "wordcut"

#if __STDC__
# define VOID void
#else
# define VOID char
#endif

#if defined (__STDC__) && __STDC__
VOID *wc_xmalloc (size_t n);
VOID *wc_xcalloc (size_t n, size_t s);
VOID *wc_xrealloc (VOID *p, size_t n);
char *wc_xstrdup (char *p);
#endif

typedef void *wc_pointer;


#define MEMCHK(mem) if( (mem) == NULL ) { fprintf(stderr,"%s:%d: Out of memory.\n",__FILE__,__LINE__); exit(1); }


#define WC_NEW_N(type,size) (type*)wc_xmalloc(sizeof(type)*(size))
#define WC_NEW(type)(type*)wc_xmalloc(sizeof(type)) 
#define WC_RENEW(type,data,size)(type *)wc_xrealloc(data,sizeof(type)*size);

typedef int wc_boolean;

#define WC_FALSE 	0
#define WC_TRUE	        (!WC_FALSE)

typedef unsigned int 		wc_uint32;
typedef unsigned char		wc_uchar;
typedef unsigned char           wc_byte;


#define WC_ERR_FILE    stderr

#define WC_HALT(msg) { fprintf(WC_ERR_FILE,"%s:%s:%d: %s\n",PACKAGE,__FILE__,__LINE__,(msg)); exit(1); }

#define WC_NEW_STR(len) (char *)WC_NEW_N(char,len+1)


#define WC_STR_DUP(target,src) { target=wc_xstrdup(src); }


#define WC_RET_NORMAL 0
#define WC_RET_ERROR -1

#define WC_RET_CHECK(cond) if(!(cond)) return WC_RET_ERROR; 

typedef int WC_STATUS;

#endif
