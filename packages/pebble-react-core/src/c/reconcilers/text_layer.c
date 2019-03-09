#include "../lib/hashmap/hashmap.h"
#include "../utils/animation_registry.h"
#include "../utils/layers_registry.h"
#include "../utils/operations.h"

#include "constants.h"
#include "layer.h"
#include "text_layer.h"

static PebbleHashMap *propsRegistry;

typedef void (*LayerAnimationCallback)(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    OperationMessage *resultOperation,
    int percent);

static LayerAnimationCallback callLayerAnimationCallback = NULL;

static GTextAlignment getTextAlignment(char *alignmentValue)
{
  if (strcmp(alignmentValue, "right") == 0)
  {
    return GTextAlignmentRight;
  }
  if (strcmp(alignmentValue, "left") == 0)
  {
    return GTextAlignmentLeft;
  }

  return GTextAlignmentCenter;
}

static void handleCanvasUpdate(Layer *layer, GContext *ctx)
{
  TextLayerPropsMessage *props = (TextLayerPropsMessage *)hash_get(propsRegistry, layer);

  if (props == 0)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "[handleCanvasUpdate] layer props not found, return");
    return;
  }

  GColor8 color = GColorWhite;

  if (props->colorChanged)
  {
    color = GColorFromHEX(props->color);
  }

  graphics_context_set_text_color(ctx, color);
  graphics_draw_text(
      ctx,
      props->text,
      fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21),
      layer_get_bounds(layer),
      GTextOverflowModeWordWrap,
      getTextAlignment(props->alignment),
      NULL);
}

void text_layer_reconciler_merge_props(
    TextLayerPropsMessage *target,
    TextLayerPropsMessage *source,
    bool force)
{
  if (source->textChanged || force)
  {
    target->textChanged = true;

    if (target->text != NULL)
    {
      free(target->text);
    }

    if (source->text == NULL)
    {
      target->text = calloc(1, sizeof(char));
      strcpy(target->text, "");
    }
    else
    {
      target->text = calloc(strlen(source->text) + 1, sizeof(char));
      strcpy(target->text, source->text);
    }
  }

  if (source->alignmentChanged || force)
  {
    target->alignmentChanged = true;

    if (target->alignment != NULL)
    {
      free(target->alignment);
    }

    if (source->alignment == NULL)
    {
      target->alignment = calloc(1, sizeof(char));
      strcpy(target->alignment, "");
    }
    else
    {
      target->alignment = calloc(strlen(source->alignment) + 1, sizeof(char));
      strcpy(target->alignment, source->alignment);
    }
  }
  if (source->colorChanged || force)
  {
    target->colorChanged = true;
    target->color = source->color;
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

static void commitUpdate(
    const char *nodeId,
    TextLayerPropsMessage *newProps)
{
  // Get layer
  Layer *layer = layer_registry_get(nodeId);
  // Merge newProps with cachedProps
  TextLayerPropsMessage *cachedProps = (TextLayerPropsMessage *)hash_get(propsRegistry, layer);
  text_layer_reconciler_merge_props(cachedProps, newProps, false);
  // Call layer reconciler to update top, left, width and height
  OperationMessage *layerOperation = malloc(sizeof(OperationMessage));
  layerOperation->operation = OPERATION_COMMIT_UPDATE;
  layerOperation->nodeType = NODE_TYPE_LAYER;
  layerOperation->nodeId = calloc(strlen(nodeId) + 1, sizeof(char));
  strcpy(layerOperation->nodeId, nodeId);
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
  OperationMessage *layerStartOperation = malloc(sizeof(OperationMessage));
  layerStartOperation->operation = OPERATION_COMMIT_UPDATE;
  layerStartOperation->nodeType = startOperation->nodeType;
  layerStartOperation->nodeId = calloc(strlen(startOperation->nodeId) + 1, sizeof(char));
  strcpy(layerStartOperation->nodeId, startOperation->nodeId);
  layerStartOperation->layerProps = startOperation->textLayerProps->layerProps;

  // Layer end Operation
  OperationMessage *layerEndOperation = malloc(sizeof(OperationMessage));
  layerEndOperation->layerProps = endOperation->textLayerProps->layerProps;

  // Layer result Operation
  OperationMessage *layerResultOperation = malloc(sizeof(OperationMessage));

  callLayerAnimationCallback = animation_registry_get_callback(NODE_TYPE_LAYER);
  callLayerAnimationCallback(layerStartOperation, layerEndOperation, layerResultOperation, percent);

  // Clear layer start Operation
  free(layerStartOperation->nodeId);
  free(layerStartOperation);
  // Clear layer end Operation
  free(layerEndOperation);

  // Temp - in reality we will create a new operation and process results
  resultOperation->textLayerProps = malloc(sizeof(TextLayerPropsMessage));
  resultOperation->textLayerProps->alignmentChanged = 0;

  TextLayerPropsMessage *startProps = startOperation->textLayerProps;
  TextLayerPropsMessage *endProps = endOperation->textLayerProps;

  if (startOperation->textLayerProps->colorChanged)
  {
    resultOperation->textLayerProps->colorChanged = 1;

    uint8_t sr = (startProps->color >> 16 & 0xff);
    uint8_t sg = (startProps->color >> 8 & 0xff);
    uint8_t sb = (startProps->color & 0xff);

    uint8_t er = (endProps->color >> 16 & 0xff);
    uint8_t eg = (endProps->color >> 8 & 0xff);
    uint8_t eb = (endProps->color & 0xff);

    uint8_t rr = (uint8_t)(sr + (er - sr) * percent / 100);
    uint8_t rg = (uint8_t)(sg + (eg - sg) * percent / 100);
    uint8_t rb = (uint8_t)(sb + (eb - sb) * percent / 100);

    resultOperation->textLayerProps->color = rr << 16 | rg << 8 | rb;
  }

  resultOperation->textLayerProps->layerPropsChanged = 1;
  resultOperation->textLayerProps->textChanged = 0;
  resultOperation->textLayerProps->layerProps = layerResultOperation->layerProps;

  // Clear layer result Operation
  free(layerResultOperation);
}

static TextLayerPropsMessage *setupCachedProps(TextLayerPropsMessage *props)
{
  TextLayerPropsMessage *cachedProps = malloc(sizeof(TextLayerPropsMessage));
  cachedProps->alignment = NULL;
  cachedProps->layerProps = NULL;
  cachedProps->text = NULL;

  text_layer_reconciler_merge_props(cachedProps, props, true);

  return cachedProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    TextLayerPropsMessage *props)
{
  TextLayerPropsMessage *cachedProps = setupCachedProps(props);

  // Layer operation
  OperationMessage *layerOperation = malloc(sizeof(OperationMessage));
  layerOperation->operation = OPERATION_APPEND_CHILD;
  layerOperation->nodeType = NODE_TYPE_LAYER;
  layerOperation->nodeId = calloc(strlen(nodeId) + 1, sizeof(char));
  strcpy(layerOperation->nodeId, nodeId);
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

  TextLayerPropsMessage *cachedProps = (TextLayerPropsMessage *)hash_get(propsRegistry, layer);
  free(cachedProps->alignment);
  free(cachedProps->layerProps);
  free(cachedProps->text);
  free(cachedProps);

  if (removeFromRegistry)
  {
    hash_remove(propsRegistry, layer);
  }

  // Layer operation
  OperationMessage *layerOperation = malloc(sizeof(OperationMessage));
  layerOperation->operation = OPERATION_REMOVE_CHILD;
  layerOperation->nodeType = NODE_TYPE_LAYER;
  layerOperation->nodeId = calloc(strlen(nodeId) + 1, sizeof(char));
  strcpy(layerOperation->nodeId, nodeId);
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

void text_layer_reconciler_init()
{
  animation_registry_add_callback(NODE_TYPE_TEXT_LAYER, handleAnimationUpdate);

  propsRegistry = hash_new();
}

void text_layer_reconciler_deinit()
{
  animation_registry_remove_callback(NODE_TYPE_TEXT_LAYER);
  hash_foreach(propsRegistry, freePropsCache);
  hash_free(propsRegistry);
}

static void clearProps(TextLayerPropsMessage *props)
{
  if (props->textChanged)
  {
    free(props->text);
  }
  if (props->alignmentChanged)
  {
    free(props->alignment);
  }
  if (props->layerPropsChanged)
  {
    free(props->layerProps);
  }
  free(props);
}

void text_layer_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage)
{
  uint8_t nodeType = operationMessage->nodeType;
  TextLayerPropsMessage *props = operationMessage->textLayerProps;
  char *nodeId = operationMessage->nodeId;

  if (nodeType != NODE_TYPE_TEXT_LAYER)
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