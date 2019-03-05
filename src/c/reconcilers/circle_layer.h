#include <pebble.h>

#include "../lib/nanopb/BatchOperationsMessage.pb.h"

void circle_layer_reconciler_merge_props(
  CircleLayerPropsMessage *cachedProps,
  CircleLayerPropsMessage *newProps,
  bool force);

void circle_layer_reconciler_init();

void circle_layer_reconciler_deinit();

void circle_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage);