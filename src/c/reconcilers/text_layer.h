#include <pebble.h>

#include "../lib/dictionary/dictionary.h"
#include "../lib/nanopb/BatchOperationsMessage.pb.h"
#include "constants.h"

void text_layer_reconciler_merge_props(
  TextLayerPropsMessage *cachedProps,
  TextLayerPropsMessage *newProps,
  bool force);

void text_layer_reconciler_init();

void text_layer_reconciler_deinit();

void text_layer_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    TextLayerPropsMessage *textLayerProps);