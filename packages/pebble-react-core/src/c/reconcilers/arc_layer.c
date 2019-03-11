#include "../lib/hashmap/hashmap.h"
#include "../utils/animation_registry.h"
#include "../utils/layers_registry.h"
#include "../utils/operations.h"

#include "constants.h"
#include "layer.h"
#include "arc_layer.h"

static PebbleHashMap *propsRegistry;

static void handleCanvasUpdate(Layer *layer, GContext *ctx)
{
  ArcLayerPropsMessage *props = (ArcLayerPropsMessage *)hash_get(propsRegistry, layer);

  if (props == 0)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "[handleCanvasUpdate] layer props not found, return");
    return;
  }

  if (props->radius == 0)
  {
    return;
  }

  const GRect frame = layer_get_frame(layer);
  const GRect rect = GRect(0, 0, frame.size.w, frame.size.h);

  GColor8 color = GColorWhite;

  if (props->colorChanged)
  {
    color = GColorFromHEX(props->color);
  }

  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_fill_color(ctx, color);

  if (props->thicknessChanged)
  {
    graphics_fill_radial(
      ctx,
      rect,
      GOvalScaleModeFitCircle,
      props->thickness,
      DEG_TO_TRIGANGLE(props->startAngle),
      DEG_TO_TRIGANGLE(props->endAngle));
  }
  else {
    graphics_context_set_stroke_width(ctx, 1);

    graphics_draw_arc(
      ctx,
      rect,
      GOvalScaleModeFitCircle,
      DEG_TO_TRIGANGLE(props->startAngle),
      DEG_TO_TRIGANGLE(props->endAngle));
  }
}

void arc_layer_reconciler_merge_props(
    ArcLayerPropsMessage *target,
    ArcLayerPropsMessage *source,
    bool force)
{
  if (source->colorChanged || force)
  {
    target->colorChanged = true;
    target->color = source->color;
  }
  if (source->radiusChanged || force)
  {
    target->radiusChanged = true;
    target->radius = source->radius;
  }
  if (source->thicknessChanged || force)
  {
    target->thicknessChanged = true;
    target->thickness = source->thickness;
  }
  if (source->startAngleChanged || force)
  {
    target->startAngleChanged = true;
    target->startAngle = source->startAngle;
  }
  if (source->endAngleChanged || force)
  {
    target->endAngleChanged = true;
    target->endAngle = source->endAngle;
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

static void calculateLayer(
    LayerPropsMessage *layerProps,
    ArcLayerPropsMessage *arcLayerProps)
{
  if (arcLayerProps->radiusChanged)
  {
    layerProps->top = arcLayerProps->layerProps->top - arcLayerProps->radius;
    layerProps->topChanged = 1;
    layerProps->left = arcLayerProps->layerProps->left - arcLayerProps->radius;
    layerProps->leftChanged = 1;
    layerProps->height = arcLayerProps->radius * 2;
    layerProps->heightChanged = 1;
    layerProps->width = layerProps->height;
    layerProps->widthChanged = 1;
  }
}

static void commitUpdate(
    const char *nodeId,
    ArcLayerPropsMessage *newProps)
{
  // Get layer
  Layer *layer = layer_registry_get(nodeId);
  // Merge newProps with cachedProps
  ArcLayerPropsMessage *cachedProps = (ArcLayerPropsMessage *)hash_get(propsRegistry, layer);
  arc_layer_reconciler_merge_props(cachedProps, newProps, false);
  // Call layer reconciler to update top, left, width and height
  OperationMessage *layerOperation = createLayerOperation(OPERATION_COMMIT_UPDATE, nodeId);
  layerOperation->layerProps = malloc(sizeof(LayerPropsMessage));

  layer_reconciler_merge_props(layerOperation->layerProps, cachedProps->layerProps, true);
  calculateLayer(layerOperation->layerProps, cachedProps);

  layer_reconciler(NULL, layerOperation);
  free(layerOperation->nodeId);
  free(layerOperation->layerProps);
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
  layerStartOperation->layerProps = startOperation->arcLayerProps->layerProps;

  // Layer end Operation
  OperationMessage *layerEndOperation = malloc(sizeof(OperationMessage));
  layerEndOperation->layerProps = endOperation->arcLayerProps->layerProps;

  // Temp - in reality we will create a new operation and process results
  resultOperation->arcLayerProps = malloc(sizeof(ArcLayerPropsMessage));

  ArcLayerPropsMessage *startProps = startOperation->arcLayerProps;
  ArcLayerPropsMessage *endProps = endOperation->arcLayerProps;

  resultOperation->arcLayerProps->radiusChanged = startProps->radiusChanged;

  if (startProps->radiusChanged)
  {
    resultOperation->arcLayerProps->radius = (int16_t)(startProps->radius + (endProps->radius - startProps->radius) * percent / 100);
  }

  resultOperation->arcLayerProps->layerPropsChanged = 0;

  // Clear layer start Operation
  free(layerStartOperation->nodeId);
  free(layerStartOperation);
  // Clear layer end Operation
  free(layerEndOperation);
}

static ArcLayerPropsMessage *setupCachedProps(ArcLayerPropsMessage *props)
{
  ArcLayerPropsMessage *cachedProps = malloc(sizeof(ArcLayerPropsMessage));
  cachedProps->layerProps = NULL;

  arc_layer_reconciler_merge_props(cachedProps, props, true);

  return cachedProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    ArcLayerPropsMessage *props)
{
  ArcLayerPropsMessage *cachedProps = setupCachedProps(props);

  // Layer operation
  OperationMessage *layerOperation = createLayerOperation(OPERATION_APPEND_CHILD, nodeId);

  layerOperation->layerProps = malloc(sizeof(LayerPropsMessage));
  layer_reconciler_merge_props(layerOperation->layerProps, cachedProps->layerProps, true);
  calculateLayer(layerOperation->layerProps, cachedProps);
  layer_reconciler(parentLayer, layerOperation);

  free(layerOperation->nodeId);
  free(layerOperation->layerProps);
  free(layerOperation);

  Layer *layer = layer_registry_get(nodeId);

  hash_add(propsRegistry, layer, cachedProps);

  layer_set_update_proc(layer, handleCanvasUpdate);
  layer_mark_dirty(layer);
}

static void removeChild(const char *nodeId, bool removeFromRegistry)
{
  Layer *layer = layer_registry_get(nodeId);

  ArcLayerPropsMessage *cachedProps = (ArcLayerPropsMessage *)hash_get(propsRegistry, layer);
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

void arc_layer_reconciler_init()
{
  animation_registry_add_callback(NODE_TYPE_ARC_LAYER, handleAnimationUpdate);

  propsRegistry = hash_new();
}

void arc_layer_reconciler_deinit()
{
  animation_registry_remove_callback(NODE_TYPE_ARC_LAYER);
  hash_foreach(propsRegistry, freePropsCache);
  hash_free(propsRegistry);
}

static void clearProps(ArcLayerPropsMessage *props)
{
  if (props->layerPropsChanged)
  {
    free(props->layerProps);
  }
  free(props);
}

void arc_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage)
{
  uint8_t nodeType = operationMessage->nodeType;
  ArcLayerPropsMessage *props = operationMessage->arcLayerProps;
  char *nodeId = operationMessage->nodeId;

  if (nodeType != NODE_TYPE_ARC_LAYER)
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