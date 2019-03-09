#include <pebble.h>

#include "../lib/nanopb/BatchOperationsMessage.pb.h"

void layer_reconciler_merge_props(
  LayerPropsMessage *cachedProps,
  LayerPropsMessage *newProps,
  bool force);

void layer_reconciler_init();

void layer_reconciler_deinit();

void layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage);