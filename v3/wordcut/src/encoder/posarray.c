#include"posarray.h"
#define DEFAULT_POS_PER_WORD  8

PosArray*
pos_array_new()
{
  PosArray* self;
  self=WC_NEW(PosArray);
  pos_array_init(self);
  return self;
}

void
pos_array_init (PosArray *self)
{
  self->size = DEFAULT_POS_PER_WORD;
  self->length = 0;
  self->array = WC_NEW_N (char, self->size);
  printf ("Array ALLOC = %X Self=%X\n",(unsigned int)self->array,
	  (unsigned int)self);
}

void
pos_array_add(PosArray* self,char pos)
{
  if (self->length+1 > self->size) WC_HALT("Too many pos per word");
  self->array[self->length]=pos;
  printf ("Array Use = %X Self=%X\n",(unsigned int)self->array,
	  (unsigned int)self);
  self->length++;    
}

void
pos_array_destroy (PosArray * self)
{
  printf ("Array Free = %X\tSelf=%X\n",(unsigned int)self->array,
	  (unsigned int)self);

  free(self->array);
}

void
pos_array_delete(PosArray* self)
{
  pos_array_destroy(self);
  free(self);
}
