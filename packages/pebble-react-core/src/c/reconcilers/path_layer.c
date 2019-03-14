#include <math.h>

#include "../lib/hashmap/hashmap.h"
#include "../utils/animation_registry.h"
#include "../utils/draw_util.h"
#include "../utils/layers_registry.h"
#include "../utils/operations.h"

#include "constants.h"
#include "layer.h"
#include "path_layer.h"

static PebbleHashMap *propsRegistry;
static PebbleHashMap *pathRegistry;

static void handleCanvasUpdate(Layer *layer, GContext *ctx)
{
  PathLayerPropsMessage *props = (PathLayerPropsMessage *)hash_get(propsRegistry, layer);

  if (props == 0)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "[handleCanvasUpdate] layer props not found, return");
    return;
  }

  GPathInfo *pathInfo = (GPathInfo *)hash_get(pathRegistry, layer);

  GPathInfo *pathInfoCopy = malloc(sizeof(GPathInfo));
  pathInfoCopy->num_points = pathInfo->num_points;
  pathInfoCopy->points = calloc(pathInfo->num_points, sizeof(GPoint));
  memcpy(pathInfoCopy->points, pathInfo->points, pathInfo->num_points * sizeof(GPoint));

  GPath *path = gpath_create(pathInfoCopy);

  GColor8 fillColor = GColorBlack;

  if (props->fillColorChanged)
  {
    fillColor = GColorFromHEX(props->fillColor);
  }

  // Fill the path:
  graphics_context_set_fill_color(ctx, fillColor);
  gpath_draw_filled(ctx, path);

  GColor8 strokeColor = GColorBlack;

  if (props->strokeColorChanged)
  {
    strokeColor = GColorFromHEX(props->strokeColor);
  }

  // Stroke the path:
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_stroke_color(ctx, strokeColor);
  gpath_draw_outline(ctx, path);
  gpath_draw_outline_open(ctx, path);

  free(pathInfoCopy->points);
  free(pathInfoCopy);
  gpath_destroy(path);
}

void path_layer_reconciler_merge_props(
    PathLayerPropsMessage *target,
    PathLayerPropsMessage *source,
    bool force)
{
  if (source->fillColorChanged || force)
  {
    target->fillColorChanged = true;
    target->fillColor = source->fillColor;
  }
  if (source->strokeColorChanged || force)
  {
    target->strokeColorChanged = true;
    target->strokeColor = source->strokeColor;
  }
  if (source->rotationAngleChanged || force)
  {
    target->rotationAngleChanged = true;
    target->rotationAngle = source->rotationAngle;
  }
  if (source->rotationPointChanged || force)
  {
    target->rotationPointChanged = true;
    target->rotationPoint = source->rotationPoint;
  }
  if (source->pointsChanged || force)
  {
    target->pointsChanged = true;

    if (target->points != NULL)
    {
      free(target->points);
    }
    if (source->points != NULL)
    {
      target->points_count = source->points_count;
      target->points = calloc(source->points_count, sizeof(PointMessage));
      memcpy(target->points, source->points, source->points_count * sizeof(PointMessage));
    }
  }
}

static void handleAnimationUpdate(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    OperationMessage *resultOperation,
    int percent)
{
  resultOperation->pathLayerProps = malloc(sizeof(PathLayerPropsMessage));
  resultOperation->pathLayerProps->fillColorChanged = 0;
  resultOperation->pathLayerProps->pointsChanged = 0;
  resultOperation->pathLayerProps->strokeColorChanged = 0;

  PathLayerPropsMessage *startProps = startOperation->pathLayerProps;
  PathLayerPropsMessage *endProps = endOperation->pathLayerProps;

  resultOperation->pathLayerProps->rotationAngleChanged = startProps->rotationAngleChanged;
  resultOperation->pathLayerProps->rotationPointChanged = 0;

  if (startProps->rotationAngleChanged)
  {
    resultOperation->pathLayerProps->rotationAngle = (int16_t)(startProps->rotationAngle + (endProps->rotationAngle - startProps->rotationAngle) * percent / 100);
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
    GPathInfo *pathInfo)
{
  GRect frame = draw_util_rect_fitting_points(pathInfo->points, pathInfo->num_points);

  // Necessary for drawing lines
  if (frame.size.w == 0)
    frame.size.w = 1;
  if (frame.size.h == 0)
    frame.size.h = 1;

  layerProps->top = frame.origin.y;
  layerProps->topChanged = 1;
  layerProps->left = frame.origin.x;
  layerProps->leftChanged = 1;
  layerProps->height = frame.size.h;
  layerProps->heightChanged = 1;
  layerProps->width = frame.size.w;
  layerProps->widthChanged = 1;
}

static GPathInfo *createPathInfo(PathLayerPropsMessage *props)
{
  GPathInfo *info = (GPathInfo *)malloc(sizeof(GPathInfo));
  info->num_points = props->points_count;
  info->points = (GPoint *)calloc(props->points_count, sizeof(GPoint));

  for (uint8_t i = 0; i < props->points_count; i++)
  {
    info->points[i] = (GPoint){props->points[i].x, props->points[i].y};

    if (props->rotationAngleChanged && props->rotationPointChanged)
    {
      GPoint center = (GPoint){props->rotationPoint.x, props->rotationPoint.y};

      info->points[i] = draw_util_rotate_point(center, info->points[i], props->rotationAngle);
    }
  }

  return info;
}

static void offsetPathInfo(GPathInfo *pathInfo, GPoint offset)
{
  for (uint8_t i = 0; i < pathInfo->num_points; i++)
  {
    pathInfo->points[i].x -= offset.x;
    pathInfo->points[i].y -= offset.y;
  }
}

static void commitUpdate(
    const char *nodeId,
    PathLayerPropsMessage *newProps)
{
  // Get layer
  Layer *layer = layer_registry_get(nodeId);
  // Merge newProps with cachedProps
  PathLayerPropsMessage *cachedProps = (PathLayerPropsMessage *)hash_get(propsRegistry, layer);
  path_layer_reconciler_merge_props(cachedProps, newProps, false);

  GPathInfo *pathInfo = createPathInfo(cachedProps);

  // Call layer reconciler to update top, left, width and height
  OperationMessage *layerOperation = createLayerOperation(OPERATION_COMMIT_UPDATE, nodeId);
  layerOperation->layerProps = (LayerPropsMessage *)malloc(sizeof(LayerPropsMessage));
  calculateLayer(layerOperation->layerProps, pathInfo);
  layer_reconciler(NULL, layerOperation);
  free(layerOperation->nodeId);
  free(layerOperation->layerProps);
  free(layerOperation);

  GRect frame = layer_get_frame(layer);

  offsetPathInfo(pathInfo, frame.origin);

  GPathInfo *cachedPathInfo = (GPathInfo *)hash_get(pathRegistry, layer);
  cachedPathInfo->num_points = pathInfo->num_points;
  free(cachedPathInfo->points);
  cachedPathInfo->points = calloc(pathInfo->num_points, sizeof(GPoint));
  memcpy(cachedPathInfo->points, pathInfo->points, pathInfo->num_points * sizeof(GPoint));
  free(pathInfo->points);
  free(pathInfo);

  layer_mark_dirty(layer);
}

static PathLayerPropsMessage *setupCachedProps(PathLayerPropsMessage *props)
{
  PathLayerPropsMessage *cachedProps = malloc(sizeof(PathLayerPropsMessage));
  cachedProps->points = NULL;

  path_layer_reconciler_merge_props(cachedProps, props, true);

  return cachedProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    PathLayerPropsMessage *props)
{
  PathLayerPropsMessage *cachedProps = setupCachedProps(props);

  GPathInfo *pathInfo = createPathInfo(cachedProps);

  // Layer operation
  OperationMessage *layerOperation = createLayerOperation(OPERATION_APPEND_CHILD, nodeId);

  layerOperation->layerProps = malloc(sizeof(LayerPropsMessage));
  calculateLayer(layerOperation->layerProps, pathInfo);
  layer_reconciler(parentLayer, layerOperation);

  free(layerOperation->nodeId);
  free(layerOperation->layerProps);
  free(layerOperation);

  Layer *layer = layer_registry_get(nodeId);

  GRect frame = layer_get_frame(layer);

  offsetPathInfo(pathInfo, frame.origin);

  hash_add(pathRegistry, layer, pathInfo);
  hash_add(propsRegistry, layer, cachedProps);

  layer_set_update_proc(layer, handleCanvasUpdate);
  layer_mark_dirty(layer);
}

static void removeChild(const char *nodeId, bool removeFromRegistry)
{
  Layer *layer = layer_registry_get(nodeId);

  PathLayerPropsMessage *cachedProps = (PathLayerPropsMessage *)hash_get(propsRegistry, layer);
  free(cachedProps->points);
  free(cachedProps);

  if (removeFromRegistry)
  {
    hash_remove(propsRegistry, layer);
  }

  GPathInfo *pathInfo = (GPathInfo *)hash_get(pathRegistry, layer);
  free(pathInfo->points);
  free(pathInfo);
  hash_remove(pathRegistry, layer);

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

void path_layer_reconciler_init()
{
  animation_registry_add_callback(NODE_TYPE_PATH_LAYER, handleAnimationUpdate);

  propsRegistry = hash_new();
  pathRegistry = hash_new();
}

void path_layer_reconciler_deinit()
{
  animation_registry_remove_callback(NODE_TYPE_PATH_LAYER);

  hash_foreach(propsRegistry, freePropsCache);
  hash_free(propsRegistry);
  hash_free(pathRegistry);
}

static void clearProps(PathLayerPropsMessage *props)
{
  if (props->pointsChanged)
  {
    free(props->points);
  }
  free(props);
}

void path_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage)
{
  uint8_t nodeType = operationMessage->nodeType;
  PathLayerPropsMessage *props = operationMessage->pathLayerProps;
  char *nodeId = operationMessage->nodeId;

  if (nodeType != NODE_TYPE_PATH_LAYER)
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