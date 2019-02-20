#include <pebble.h>

#include "../lib/dictionary/dictionary.h"

char * layer_registry_get_node_id(Layer *layer);
int layer_registry_has(const char *nodeId);
Layer * layer_registry_get(const char *nodeId);
void *layer_registry_get_reconciler(Layer *layer);
void layer_registry_add_reconciler(Layer *layer, void *reconciler);
void layer_registry_add(const char *nodeId, Layer *layer);
void layer_registry_deinit();
void layer_registry_init();
void layer_registry_remove_reconciler(Layer *layer);
void layer_registry_remove(const char *nodeId);