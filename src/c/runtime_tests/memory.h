#include <pebble.h>

#include "../lib/dictionary/dictionary.h"
#include "../lib/json/jsonparser.h"

void assert_dict_add_remove();
void assert_dict_new_free();

void assert_json_object_parse();
void assert_json_array_parse();

void assert_substr();

void assert_text_reconciler_init_deinit();
void assert_text_reconciler_add_remove(Layer *parentLayer);
void assert_text_reconciler_remove_leftovers(Layer *parentLayer);