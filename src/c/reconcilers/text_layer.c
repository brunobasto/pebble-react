#include "text_layer.h"
#include "../utils/layer_props_utils.h"
#include "../utils/layers_registry.h"

static PebbleDictionary *layerPropsRegistry;

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

  APP_LOG(APP_LOG_LEVEL_INFO, "trying to render text");

  TextLayerPropsMessage *textLayerProps = (TextLayerPropsMessage *)dict_get(layerPropsRegistry, nodeId);

  graphics_context_set_text_color(ctx, GColorWhite);

  APP_LOG(APP_LOG_LEVEL_INFO, "rendering text %s", textLayerProps->text);

  graphics_draw_text(
      ctx,
      textLayerProps->text,
      fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21),
      layer_get_bounds(layer),
      GTextOverflowModeWordWrap,
      getTextAlignment(textLayerProps->alignment),
      NULL);
}

static void mergeCachedProps(TextLayerPropsMessage *cachedProps, TextLayerPropsMessage *newProps) {
  if (newProps->textChanged) {
    strcpy(cachedProps->text, newProps->text);
  }
  if (newProps->alignmentChanged) {
    strcpy(cachedProps->alignment, newProps->alignment);
  }
  if (newProps->leftChanged) {
    cachedProps->left = newProps->left;
  }
  if (newProps->topChanged) {
    cachedProps->top = newProps->top;
  }
  if (newProps->widthChanged) {
    cachedProps->width = newProps->width;
  }
  if (newProps->heightChanged) {
    cachedProps->height = newProps->height;
  }
}

static void commitUpdate(
    const char *nodeId,
    TextLayerPropsMessage *newProps)
{
  APP_LOG(
    APP_LOG_LEVEL_DEBUG,
    "commitUpdate (%s, %d, %d, %d, %d)",
    newProps->text,
    newProps->left,
    newProps->top,
    newProps->width,
    newProps->height);

  // Merge newProps with cachedProps
  TextLayerPropsMessage *cachedProps = (TextLayerPropsMessage *)dict_get(layerPropsRegistry, nodeId);

  mergeCachedProps(cachedProps, newProps);

  APP_LOG(
    APP_LOG_LEVEL_DEBUG,
    "commitUpdate cache (%s, %d, %d, %d, %d)",
    cachedProps->text,
    cachedProps->left,
    cachedProps->top,
    cachedProps->width,
    cachedProps->height);

  Layer *layer = layer_registry_get(nodeId);

  GRect frame = GRect(
    cachedProps->left,
    cachedProps->top,
    cachedProps->width,
    cachedProps->height
  );

  layer_set_frame(layer, frame);
}

static TextLayerPropsMessage * setupCachedProps(TextLayerPropsMessage *textLayerProps) {
  TextLayerPropsMessage *cachedProps = malloc(sizeof(TextLayerPropsMessage));

  TextLayerPropsMessage defaultProps = TextLayerPropsMessage_init_default;

  strcpy(cachedProps->text, defaultProps.text);
  strcpy(cachedProps->text, defaultProps.text);

  cachedProps->top = defaultProps.top;
  cachedProps->left = defaultProps.left;
  cachedProps->height = defaultProps.height;
  cachedProps->width = defaultProps.width;

  mergeCachedProps(cachedProps, textLayerProps);

  return cachedProps;
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    TextLayerPropsMessage *textLayerProps)
{
  APP_LOG(
    APP_LOG_LEVEL_DEBUG,
    "adding text (%s, %d, %d, %d, %d)",
    textLayerProps->text,
    textLayerProps->left,
    textLayerProps->top,
    textLayerProps->width,
    textLayerProps->height);

  TextLayerPropsMessage *cachedProps = setupCachedProps(textLayerProps);

  dict_add(layerPropsRegistry, nodeId, cachedProps);

  APP_LOG(APP_LOG_LEVEL_INFO, "managed to setup");

  GRect frame = GRect(
    textLayerProps->left,
    textLayerProps->top,
    textLayerProps->width,
    textLayerProps->height
  );

  Layer *layer = layer_create(frame);

  layer_registry_add(nodeId, layer);
  layer_registry_add_reconciler(layer, commitUpdate);
  layer_add_child(parentLayer, layer);
  layer_set_update_proc(layer, handleCanvasUpdate);
}

static void removeChild(const char *nodeId)
{
  if (layer_registry_has(nodeId))
  {
    Layer *layer = layer_registry_get(nodeId);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "removing %p from %s", layer, nodeId);

    layer_remove_child_layers(layer);
    layer_remove_from_parent(layer);
    layer_registry_remove_reconciler(layer);
    layer_registry_remove(nodeId);

    layer_destroy(layer);
  }

  TextLayerPropsMessage *cachedProps = (TextLayerPropsMessage *)dict_get(layerPropsRegistry, nodeId);
  free(cachedProps);
  dict_remove(layerPropsRegistry, nodeId);
}

static void freePropsCache(char *key, void* value) {
  removeChild(key);
}

// Public

void text_layer_reconciler_init()
{
  layerPropsRegistry = dict_new();
}

void text_layer_reconciler_deinit()
{
  dict_foreach(layerPropsRegistry, freePropsCache);
  dict_free(layerPropsRegistry);
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
    removeChild(nodeId);
    break;
  default:
    break;
  }
}