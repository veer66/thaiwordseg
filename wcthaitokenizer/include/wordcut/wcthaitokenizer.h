#ifndef __WC_THAI_TOKENIZER_H__
#define __WC_THAI_TOKENIZER_H__
typedef void (*wc_token_callback_t)(const char *,size_t,void*);
void wc_thai_tokenizer(const char *source,
	     wc_token_callback_t thai_callback,
	     void* thai_support_data,
	     wc_token_callback_t default_callback,
	     void* default_support_data);
#endif
