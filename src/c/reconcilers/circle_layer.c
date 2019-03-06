#include "../lib/hashmap/hashmap.h"
#include "../utils/animation_registry.h"
#include "../utils/layers_registry.h"
#include "../utils/operations.h"

#include "constants.h"
#include "layer.h"
#include "circle_layer.h"

static PebbleHashMap *propsRegistry;

typedef void (*LayerAnimationCallback)(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    OperationMessage *resultOperation,
    int percent);

static LayerAnimationCallback callLayerAnimationCallback = NULL;

static void handleCanvasUpdate(Layer *layer, GContext *ctx)
{
  CircleLayerPropsMessage *props = (CircleLayerPropsMessage *)hash_get(propsRegistry, layer);

  if (props == 0)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "[handleCanvasUpdate] layer props not found, return");
    return;
  }

  GPoint center = GPoint(
      props->radius,
      props->radius);

  // Draw the outline of a circle
  graphics_draw_circle(ctx, center, props->radius);

  // Fill a circle
  graphics_fill_circle(ctx, center, props->radius);
}

void circle_layer_reconciler_merge_props(
    CircleLayerPropsMessage *target,
    CircleLayerPropsMessage *source,
    bool force)
{
  if (source->radiusChanged || force)
  {
    target->radiusChanged = true;
    target->radius = source->radius;
  }

  if (source->layerPropsChanged || force)
  {
    target->layerPropsChanged = true;

    if (target->layerProps == NULL)
    {
      target->layerProps = malloc(sizeof(LayerPropsMessage));
    }

    if (source->layerProps != NULL)
    {
      layer_reconciler_merge_props(target->layerProps, source->layerProps, force);
    }
  }
}

static OperationMessage *createLayerOperation(
    uint8_t operationType,
    const char *nodeId)
{

  OperationMessage *layerOperation = malloc(sizeof(OperationMessage));
  layerOperation->operation = operationType;
  layerOperation->nodeType = NODE_TYPE_LAYER;
  layerOperation->nodeId = calloc(strlen(nodeId) + 1, sizeof(char));
  strcpy(layerOperation->nodeId, nodeId);

  return layerOperation;
}

static void commitUpdate(
    const char *nodeId,
    CircleLayerPropsMessage *newProps)
{
  // Get layer
  Layer *layer = layer_registry_get(nodeId);
  // Merge newProps with cachedProps
  CircleLayerPropsMessage *cachedProps = (CircleLayerPropsMessage *)hash_get(propsRegistry, layer);
  circle_layer_reconciler_merge_props(cachedProps, newProps, false);
  // Call layer reconciler to update top, left, width and height
  OperationMessage *layerOperation = createLayerOperation(OPERATION_COMMIT_UPDATE, nodeId);
  layerOperation->layerProps = cachedProps->layerProps;
  layer_reconciler(NULL, layerOperation);
  free(layerOperation->nodeId);
  free(layerOperation);

  layer_mark_dirty(layer);
}

static void handleAnimationUpdate(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    OperationMessage *resultOperation,
    int percent)
{
  // Layer start Operation
  OperationMessage *layerStartOperation = createLayerOperation(OPERATION_COMMIT_UPDATE, startOperation->nodeId);
  layerStartOperation->layerProps = startOperation->circleLayerProps->layerProps;

  // Layer end Operation
  OperationMessage *layerEndOperation = malloc(sizeof(OperationMessage));
  layerEndOperation->layerProps = endOperation->circleLayerProps->layerProps;

  // Layer result Operation
  OperationMessage *layerResultOperation = malloc(sizeof(OperationMessage));

  // Call Layer animation handler
  callLayerAnimationCallback = animation_registry_get_callback(NODE_TYPE_LAYER);
  callLayerAnimationCallback(layerStartOperation, layerEndOperation, layerResultOperation, percent);

  // Temp - in reality we will create a new operation and process results
  resultOperation->circleLayerProps = malloc(sizeof(CircleLayerPropsMessage));

  CircleLayerPropsMessage *startProps = startOperation->circleLayerProps;
  CircleLayerPropsMessage *endProps = endOperation->circleLayerProps;

  resultOperation->circleLayerProps->radiusChanged = startProps->radiusChanged;

  if (startProps->radiusChanged)
  {
    Layer *layer = layer_registry_get(layerStartOperation->nodeId);

    CircleLayerPropsMessage *props = (CircleLayerPropsMessage *)hash_get(propsRegistry, layer);

    resultOperation->circleLayerProps->radius = (int16_t)(startProps->radius + (endProps->radius - startProps->radius) * percent / 100);

    int8_t radiusDiff = resultOperation->circleLayerProps->radius - props->radius;

    layerResultOperation->layerProps->top = props->layerProps->top - radiusDiff;
    layerResultOperation->layerProps->topChanged = 1;

    layerResultOperation->layerProps->left = props->layerProps->left - radiusDiff;
    layerResultOperation->layerProps->leftChanged = 1;
  }

  resultOperation->circleLayerProps->layerPropsChanged = 1;
  resultOperation->circleLayerProps->layerProps = layerResultOperation->layerProps;

  // Clear layer start Operation
  free(layerStartOperation->nodeId);
  free(layerStartOperation);
  // Clear layer end Operation
  free(layerEndOperation);
  // Clear layer result Operation
  free(layerResultOperation);
}

static CircleLayerPropsMessage *setupCachedProps(CircleLayerPropsMessage *props)
{
  CircleLayerPropsMessage *cachedProps = malloc(sizeof(CircleLayerPropsMessage));
  cachedProps->layerProps = NULL;

  circle_layer_reconciler_merge_props(cachedProps, props, true);

  return cachedProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    CircleLayerPropsMessage *props)
{
  CircleLayerPropsMessage *cachedProps = setupCachedProps(props);

  // Layer operation
  OperationMessage *layerOperation = createLayerOperation(OPERATION_APPEND_CHILD, nodeId);
  layerOperation->layerProps = cachedProps->layerProps;

  layer_reconciler(parentLayer, layerOperation);
  free(layerOperation->nodeId);
  free(layerOperation);

  Layer *layer = layer_registry_get(nodeId);

  hash_add(propsRegistry, layer, cachedProps);

  layer_set_update_proc(layer, handleCanvasUpdate);
  layer_mark_dirty(layer);
}

static void removeChild(const char *nodeId, bool removeFromRegistry)
{
  Layer *layer = layer_registry_get(nodeId);

  CircleLayerPropsMessage *cachedProps = (CircleLayerPropsMessage *)hash_get(propsRegistry, layer);
  free(cachedProps->layerProps);
  free(cachedProps);

  if (removeFromRegistry)
  {
    hash_remove(propsRegistry, layer);
  }

  // Layer operation
  OperationMessage *layerOperation = createLayerOperation(OPERATION_REMOVE_CHILD, nodeId);
  layer_reconciler(NULL, layerOperation);
  free(layerOperation->nodeId);
  free(layerOperation);
}

static void freePropsCache(void *key, void *value)
{
  char *nodeId = layer_registry_get_node_id((Layer *)key);

  removeChild(nodeId, false);
}

// Public

void circle_layer_reconciler_init()
{
  animation_registry_add_callback(NODE_TYPE_CIRCLE_LAYER, handleAnimationUpdate);

  propsRegistry = hash_new();
}

void circle_layer_reconciler_deinit()
{
  animation_registry_remove_callback(NODE_TYPE_CIRCLE_LAYER);
  hash_foreach(propsRegistry, freePropsCache);
  hash_free(propsRegistry);
}

static void clearProps(CircleLayerPropsMessage *props)
{
  if (props->layerPropsChanged)
  {
    free(props->layerProps);
  }
  free(props);
}

void circle_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage)
{
  uint8_t nodeType = operationMessage->nodeType;
  CircleLayerPropsMessage *props = operationMessage->circleLayerProps;
  char *nodeId = operationMessage->nodeId;

  if (nodeType != NODE_TYPE_CIRCLE_LAYER)
  {
    return;
  }

  switch (operationMessage->operation)
  {
  case OPERATION_APPEND_CHILD:
    appendChild(parentLayer, nodeId, props);
    break;
  case OPERATION_COMMIT_UPDATE:
    commitUpdate(nodeId, props);
    break;
  case OPERATION_REMOVE_CHILD:
    removeChild(nodeId, true);
    break;
  case OPERATION_CLEAR_PROPS:
    clearProps(props);
    break;
  default:
    break;
  }
}