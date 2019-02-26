#include "text_layer.h"
#include "../utils/animation_registry.h"
#include "../utils/layer_props_utils.h"
#include "../utils/layers_registry.h"

static PebbleDictionary *propsRegistry;

static GTextAlignment getTextAlignment(char *alignmentValue)
{
  if (strcmp(alignmentValue, "center") == 0)
  {
    return GTextAlignmentCenter;
  }
  if (strcmp(alignmentValue, "left") == 0)
  {
    return GTextAlignmentLeft;
  }

  return GTextAlignmentRight;
}

static void handleCanvasUpdate(Layer *layer, GContext *ctx)
{
  char *nodeId = layer_registry_get_node_id(layer);

  if (nodeId == NULL)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "[handleCanvasUpdate] laye not found, return");
    return;
  }

  TextLayerPropsMessage *props = (TextLayerPropsMessage *)dict_get(propsRegistry, nodeId);

  graphics_context_set_text_color(ctx, GColorWhite);
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
    if (target->text != NULL) {
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
      free(source->text);
    }
  }
  if (source->alignmentChanged || force)
  {
    target->alignmentChanged = true;
    if (target->alignment != NULL) {
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
      free(source->alignment);
    }
  }
  if (source->leftChanged || force)
  {
    target->leftChanged = true;
    target->left = source->left;
  }
  if (source->topChanged || force)
  {
    target->topChanged = true;
    target->top = source->top;
  }
  if (source->widthChanged || force)
  {
    target->widthChanged = true;
    target->width = source->width;
  }
  if (source->heightChanged || force)
  {
    target->heightChanged = true;
    target->height = source->height;
  }
}

static void commitUpdate(
    const char *nodeId,
    TextLayerPropsMessage *newProps)
{
  // Merge newProps with cachedProps
  TextLayerPropsMessage *cachedProps = (TextLayerPropsMessage *)dict_get(propsRegistry, nodeId);

  text_layer_reconciler_merge_props(cachedProps, newProps, false);

  Layer *layer = layer_registry_get(nodeId);

  GRect frame = GRect(
      cachedProps->left,
      cachedProps->top,
      cachedProps->width,
      cachedProps->height);

  layer_set_frame(layer, frame);
  layer_mark_dirty(layer);
}

static void handleAnimationUpdate(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    int percent)
{
  TextLayerPropsMessage *startProps = startOperation->textLayerProps;
  TextLayerPropsMessage *endProps = endOperation->textLayerProps;
  TextLayerPropsMessage resultProps = TextLayerPropsMessage_init_zero;

  if (startProps->topChanged)
  {
    resultProps.top = (int16_t)(startProps->top + (endProps->top - startProps->top) * percent / 100);
    resultProps.topChanged = 1;
  }
  if (startProps->leftChanged)
  {
    resultProps.left = (int16_t)(startProps->left + (endProps->left - startProps->left) * percent / 100);
    resultProps.leftChanged = 1;
  }

  commitUpdate(startOperation->nodeId, &resultProps);
  commitUpdate(startOperation->nodeId, &resultProps);
}

static TextLayerPropsMessage *setupCachedProps(TextLayerPropsMessage *props)
{
  TextLayerPropsMessage *cachedProps = malloc(sizeof(TextLayerPropsMessage));
  cachedProps->text = NULL;
  cachedProps->alignment = NULL;

  text_layer_reconciler_merge_props(cachedProps, props, true);

  return cachedProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    TextLayerPropsMessage *props)
{
  TextLayerPropsMessage *cachedProps = setupCachedProps(props);

  dict_add(propsRegistry, nodeId, cachedProps);

  APP_LOG(
      APP_LOG_LEVEL_DEBUG,
      "appendChild cache (%s, %d, %d, %d, %d)",
      cachedProps->text,
      cachedProps->left,
      cachedProps->top,
      cachedProps->width,
      cachedProps->height);

  GRect frame = GRect(
      props->left,
      props->top,
      props->width,
      props->height);

  Layer *layer = layer_create(frame);

  layer_registry_add(nodeId, layer);
  layer_registry_add_reconciler(layer, commitUpdate);
  layer_set_update_proc(layer, handleCanvasUpdate);
  layer_add_child(parentLayer, layer);
  layer_mark_dirty(layer);
}

static void removeChild(const char *nodeId, bool removeFromRegistry)
{
  if (layer_registry_has(nodeId))
  {
    Layer *layer = layer_registry_get(nodeId);

    layer_remove_child_layers(layer);
    layer_remove_from_parent(layer);
    layer_registry_remove_reconciler(layer);
    layer_registry_remove(nodeId);

    layer_destroy(layer);
  }

  TextLayerPropsMessage *cachedProps = (TextLayerPropsMessage *)dict_get(propsRegistry, nodeId);
  free(cachedProps->text);
  free(cachedProps->alignment);
  free(cachedProps);

  if (removeFromRegistry)
  {
    dict_remove(propsRegistry, nodeId);
  }
}

static void freePropsCache(char *key, void *value)
{
  removeChild(key, false);
}

// Public

void text_layer_reconciler_init()
{
  animation_registry_add_callback(NODE_TYPE_TEXT_LAYER, handleAnimationUpdate);

  propsRegistry = dict_new();
}

void text_layer_reconciler_deinit()
{
  animation_registry_remove_callback(NODE_TYPE_TEXT_LAYER);
  dict_foreach(propsRegistry, freePropsCache);
  dict_free(propsRegistry);
}

void text_layer_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    TextLayerPropsMessage *textLayerProps)
{
  if (nodeType != NODE_TYPE_TEXT_LAYER)
  {
    return;
  }

  switch (operation)
  {
  case OPERATION_APPEND_CHILD:
    appendChild(parentLayer, nodeId, textLayerProps);
    break;
  case OPERATION_COMMIT_UPDATE:
    commitUpdate(nodeId, textLayerProps);
    break;
  case OPERATION_REMOVE_CHILD:
    removeChild(nodeId, true);
    break;
  default:
    break;
  }
}