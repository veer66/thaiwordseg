#ifndef __WORD_H__
#define __WORD_H__

#include <wordcut/wccommon.h>
#include <wordcut/wcstring.h>
typedef struct word_t {
  WcString surface;
  WcString *pos;
  size_t pos_count;
} Word;

#endif
