#include <pebble.h>

#include "../lib/nanopb/BatchOperationsMessage.pb.h"

void path_layer_reconciler_merge_props(
  PathLayerPropsMessage *cachedProps,
  PathLayerPropsMessage *newProps,
  bool force);

void path_layer_reconciler_init();

void path_layer_reconciler_deinit();

void path_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage);