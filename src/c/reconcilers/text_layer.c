#include "text_layer.h"
#include "utils/layer_reconciler_utils.h"

static PebbleDictionary *layerPropsDict;

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

static void setupInitialProps(const char *nodeId, PebbleDictionary *propsDict)
{
  if (!dict_has(propsDict, "font"))
  {
    dict_add(propsDict, "font", fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  }
  if (!dict_has(propsDict, "children"))
  {
    dict_add(propsDict, "children", (void *)"");
  }
  if (!dict_has(propsDict, "alignment"))
  {
    dict_add(propsDict, "alignment", (void *)"center");
  }
  if (!dict_has(propsDict, "color"))
  {
    dict_add(propsDict, "color", &GColorWhite);
  }
}

static void handleCanvasUpdate(Layer *layer, GContext *ctx)
{
  char *nodeId = layer_registry_get_node_id(layer);

  if (nodeId == NULL)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "not found, return");
    return;
  }

  PebbleDictionary *propsDict = (PebbleDictionary *)dict_get(layerPropsDict, nodeId);

  graphics_context_set_text_color(ctx, *(GColor *)dict_get(propsDict, "color"));

  graphics_draw_text(
      ctx,
      (char *)dict_get(propsDict, "children"),
      (GFont)dict_get(propsDict, "font"),
      layer_get_bounds(layer),
      GTextOverflowModeWordWrap,
      getTextAlignment((char *)dict_get(propsDict, "alignment")),
      NULL);
}

static void commitUpdate(
    const char *nodeId,
    PebbleDictionary *newProps)
{
  // Merge newProps with cachedProps
  PebbleDictionary *cachedProps = (PebbleDictionary *)dict_get(layerPropsDict, nodeId);
  layer_utils_merge_props(cachedProps, newProps);

  Layer *layer = layer_registry_get(nodeId);
  set_layer_frame_from_props(layer, cachedProps);
}

static void appendChild(
    Layer *parentLayer,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  setupInitialProps(nodeId, propsDict);

  GRect frame = get_layer_frame_from_props(propsDict);
  Layer *layer = layer_create(frame);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "adding %p to %s", layer, nodeId);
  layer_registry_add(nodeId, layer);
  layer_registry_add_reconciler(layer, commitUpdate);
  layer_add_child(parentLayer, layer);
  layer_set_update_proc(layer, handleCanvasUpdate);
  dict_add(layerPropsDict, nodeId, propsDict);
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

    // char *testkey = "test";
    // APP_LOG(APP_LOG_LEVEL_INFO, "before adding to dict %d", heap_bytes_used());
    // layer_registry_add(testkey, layer);
    // APP_LOG(APP_LOG_LEVEL_INFO, "before dict_has %d", heap_bytes_used());
    // layer_registry_has(testkey);
    // APP_LOG(APP_LOG_LEVEL_INFO, "after dict_has to dict %d", heap_bytes_used());
    // layer_registry_remove(testkey);
    // APP_LOG(APP_LOG_LEVEL_INFO, "after removing from dict %d", heap_bytes_used());

    layer_destroy(layer);
  }

  PebbleDictionary *cachedProps = (PebbleDictionary *)dict_get(layerPropsDict, nodeId);
  dict_remove(cachedProps, "alignment");
  dict_remove(cachedProps, "children");
  dict_remove(cachedProps, "color");
  dict_remove(cachedProps, "height");
  dict_remove(cachedProps, "left");
  dict_remove(cachedProps, "top");
  dict_remove(cachedProps, "width");
  dict_free(cachedProps);
  dict_remove(layerPropsDict, nodeId);
}

// Public

void text_layer_reconciler_init()
{
  layerPropsDict = dict_new();
}

void text_layer_reconciler_deinit()
{
  dict_free(layerPropsDict);
}

void text_layer_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  if (nodeType != NODE_TYPE_TEXT_LAYER)
  {
    return;
  }

  switch (operation)
  {
  case OPERATION_APPEND_CHILD:
    appendChild(parentLayer, nodeId, propsDict);
    break;
  case OPERATION_COMMIT_UPDATE:
    commitUpdate(nodeId, propsDict);
    break;
  case OPERATION_REMOVE_CHILD:
    removeChild(nodeId);
    break;
  default:
    break;
  }
}