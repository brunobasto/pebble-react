#include <pebble.h>
#include "../../lib/dictionary/dictionary.h"

void set_layer_frame_from_props(Layer *layer, PebbleDictionary *propsDict);

Layer * layer_registry_get(const char *nodeId);

char * layer_registry_get_node_id(Layer *layer);

void layer_utils_merge_props(PebbleDictionary *dest, PebbleDictionary *source);

void layer_registry_add(const char *nodeId, Layer *layer);

void layer_registry_remove(const char *nodeId);

int layer_registry_has(const char *nodeId);

void layer_registry_init();

void layer_registry_deinit();

void *layer_registry_get_reconciler(Layer *layer);

void layer_registry_add_reconciler(Layer *layer, void *reconciler);

GRect get_layer_frame_from_props(PebbleDictionary *propsDict);