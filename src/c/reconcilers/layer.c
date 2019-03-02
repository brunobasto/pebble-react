#include "constants.h"
#include "layer.h"

#include "../utils/animation_registry.h"
#include "../utils/layers_registry.h"

void layer_reconciler_merge_props(
    LayerPropsMessage *target,
    LayerPropsMessage *source,
    bool force)
{
  target->heightChanged = source->heightChanged;
  target->leftChanged = source->leftChanged;
  target->topChanged = source->topChanged;
  target->widthChanged = source->widthChanged;

  if (source->leftChanged || force)
  {
    // target->leftChanged = true;
    target->left = source->left;
  }
  if (source->topChanged || force)
  {
    // target->topChanged = true;
    target->top = source->top;
  }
  if (source->widthChanged || force)
  {
    // target->widthChanged = true;
    target->width = source->width;
  }
  if (source->heightChanged || force)
  {
    // target->heightChanged = true;
    target->height = source->height;
  }
}

static void commitUpdate(
    const char *nodeId,
    LayerPropsMessage *newProps)
{
  Layer *layer = layer_registry_get(nodeId);

  GRect frame = layer_get_frame(layer);

  if (newProps->topChanged) {
    frame.origin.y = newProps->top;
  }
  if (newProps->leftChanged) {
    frame.origin.x = newProps->left;
  }
  if (newProps->heightChanged) {
    frame.size.h = newProps->height;
  }
  if (newProps->widthChanged) {
    frame.size.w = newProps->width;
  }

  layer_set_frame(layer, frame);
  layer_mark_dirty(layer);
}

static void handleAnimationUpdate(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    OperationMessage *resultOperation,
    int percent)
{
  LayerPropsMessage *startProps = startOperation->layerProps;
  LayerPropsMessage *endProps = endOperation->layerProps;

  // Result props
  LayerPropsMessage *resultProps = malloc(sizeof(LayerPropsMessage));
  resultProps->topChanged = startProps->topChanged;
  resultProps->leftChanged = startProps->leftChanged;
  resultProps->heightChanged = startProps->heightChanged;
  resultProps->widthChanged = startProps->widthChanged;

  if (startProps->topChanged)
  {
    resultProps->top = (int16_t)(startProps->top + (endProps->top - startProps->top) * percent / 100);
  }
  if (startProps->leftChanged)
  {
    resultProps->left = (int16_t)(startProps->left + (endProps->left - startProps->left) * percent / 100);
  }

  resultOperation->layerProps = resultProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    LayerPropsMessage *props)
{
  GRect frame = GRect(
      props->left,
      props->top,
      props->width,
      props->height);

  Layer *layer = layer_create(frame);

  layer_registry_add(nodeId, layer);
  layer_add_child(parentLayer, layer);
  layer_mark_dirty(layer);
}

static void removeChild(const char *nodeId, bool removeFromRegistry)
{
  Layer *layer = layer_registry_get(nodeId);

  layer_remove_child_layers(layer);
  layer_remove_from_parent(layer);
  layer_registry_remove(nodeId);
  layer_destroy(layer);
}

// Public

void layer_reconciler_init()
{
  animation_registry_add_callback(NODE_TYPE_LAYER, handleAnimationUpdate);
}

void layer_reconciler_deinit()
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Deinit layer");
  animation_registry_remove_callback(NODE_TYPE_LAYER);
}

static void clearProps(LayerPropsMessage *props) {
  free(props);
}

void layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage)
{
  uint8_t nodeType = operationMessage->nodeType;
  LayerPropsMessage *props = operationMessage->layerProps;
  char *nodeId = operationMessage->nodeId;

  if (nodeType != NODE_TYPE_LAYER)
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
  default:
    break;
  }
}