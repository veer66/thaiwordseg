#ifndef __WI_COMMON_H__
#define __WI_COMMON_H__

#define MEMCHK(mem) if( (mem) == NULL ) { \
  fprintf(stderr,"%s:%d: Out of memory.\n",__FILE__,__LINE__); exit(1); }

typedef int wi_boolean;

#define WI_FALSE 	0
#define WI_TRUE		1

#endif
