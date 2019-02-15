#include <pebble.h>

#include "../lib/dictionary/dictionary.h"
#include "constants.h"

void text_layer_reconciler_init();

void text_layer_reconciler_deinit();

void text_layer_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict);