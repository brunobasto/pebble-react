#include <pebble.h>

#include "../lib/dictionary/dictionary.h"
#include "../lib/nanopb/BatchOperationsMessage.pb.h"
#include "constants.h"

void animation_reconciler_init();

void animation_reconciler_deinit();

void animation_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    AnimationPropsMessage *props);