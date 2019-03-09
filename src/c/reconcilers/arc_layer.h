#include <pebble.h>

#include "../lib/nanopb/BatchOperationsMessage.pb.h"

void arc_layer_reconciler_merge_props(
  ArcLayerPropsMessage *cachedProps,
  ArcLayerPropsMessage *newProps,
  bool force);

void arc_layer_reconciler_init();

void arc_layer_reconciler_deinit();

void arc_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage);