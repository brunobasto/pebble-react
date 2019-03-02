#include <pebble.h>

#include "../lib/nanopb/BatchOperationsMessage.pb.h"

void text_layer_reconciler_merge_props(
  TextLayerPropsMessage *cachedProps,
  TextLayerPropsMessage *newProps,
  bool force);

void text_layer_reconciler_init();

void text_layer_reconciler_deinit();

void text_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage);