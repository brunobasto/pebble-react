#include <pebble.h>

#include "../lib/dictionary/dictionary.h"

void layer_props_utils_set_frame_from_props(Layer *layer, PebbleDictionary *propsDict);

void layer_props__utils_merge_props(PebbleDictionary *dest, PebbleDictionary *source);

GRect layer_props_utils_get_frame_from_props(PebbleDictionary *propsDict);