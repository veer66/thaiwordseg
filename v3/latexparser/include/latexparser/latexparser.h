#ifndef __LATEX_PARSER_H__
#define __LATEX_PARSER_H__
#define LATEX_PARSER_MAX_DEPTH  4096
#include <glib.h>

void lxpa_init();
int lxpa_input(const gchar* filename);
void lxpa_destroy();

typedef gpointer (*lxpa_callback)(gchar *txt,gpointer data);
void lxpa_parse();
void lxpa_set_callback(lxpa_callback symbol_callback,gpointer symbol_data, /* 1 */
		       lxpa_callback comment_callback,gpointer comment_data, /* 2 */
		       lxpa_callback env_begin_callback,gpointer env_begin_data, /* 3 */
		       lxpa_callback env_end_callback,gpointer env_end_data,/* 4 */
		       lxpa_callback tag_callback,gpointer tag_data,/* 5 */
		       lxpa_callback tag2_callback,gpointer tag2_data,/* 6 */
		       lxpa_callback block_begin_callback,gpointer block_begin_data,/* 7 */
		       lxpa_callback block_end_callback,gpointer block_end_data, /* 8 */
		       lxpa_callback content_callback,gpointer content_data,/* 9 */
		       lxpa_callback newline_callback,gpointer newline_data,/* 10 */
		       lxpa_callback other_callback,gpointer other_data);/* 11 */
#endif
