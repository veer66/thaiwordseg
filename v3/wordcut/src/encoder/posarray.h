#ifndef __POS_ARRAY_H__
#define __POS_ARRAY_H__

#include<wordcut/wccommon.h>

typedef struct pos_array_t
{
  size_t size, length;
  unsigned char *array;
}
PosArray;

PosArray* pos_array_new();
void pos_array_init (PosArray * self);
void pos_array_add(PosArray* self,char pos);
void pos_array_destroy (PosArray * self);
void pos_array_delete(PosArray* self);

#endif
