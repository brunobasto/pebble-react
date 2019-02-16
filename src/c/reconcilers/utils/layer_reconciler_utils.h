#include <pebble.h>
#include "../../lib/dictionary/dictionary.h"

void set_layer_frame_from_props(Layer *layer, PebbleDictionary *propsDict);

GRect get_layer_frame_from_props(PebbleDictionary *propsDict);