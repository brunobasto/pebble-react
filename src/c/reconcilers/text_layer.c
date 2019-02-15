#include "text_layer.h"

PebbleDictionary *textLayersDict;

static void setTextLayerProperties(TextLayer *textLayer, PebbleDictionary *propsDict)
{
  uint16_t top = atoi((char *)dict_get(propsDict, "top"));
  uint16_t left = atoi((char *)dict_get(propsDict, "left"));
  uint16_t width = atoi((char *)dict_get(propsDict, "width"));
  uint16_t height = atoi((char *)dict_get(propsDict, "height"));
  char *text = (char *)dict_get(propsDict, "text");

  text_layer_set_font(textLayer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_background_color(textLayer, GColorClear);
  text_layer_set_text_color(textLayer, GColorWhite);
  text_layer_set_text_alignment(textLayer, GTextAlignmentCenter);

  text_layer_set_text(textLayer, text);

  Layer *layer = text_layer_get_layer(textLayer);
  layer_set_frame(layer, GRect(left, top, width, height));
}

static void appendChild(
    Layer *parentLayer,
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  uint16_t top = atoi((char *)dict_get(propsDict, "top"));
  uint16_t left = atoi((char *)dict_get(propsDict, "left"));
  uint16_t width = atoi((char *)dict_get(propsDict, "width"));
  uint16_t height = atoi((char *)dict_get(propsDict, "height"));
  TextLayer *textLayer = text_layer_create(GRect(left, top, width, height));
  layer_add_child(parentLayer, text_layer_get_layer(textLayer));
  dict_add(textLayersDict, nodeId, textLayer);

  setTextLayerProperties(textLayer, propsDict);
}

static void commitUpdate(
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  TextLayer *textLayer = (TextLayer *)dict_get(textLayersDict, nodeId);

  setTextLayerProperties(textLayer, propsDict);
}

static void removeChild(
    const uint16_t nodeType,
    const char *nodeId)
{
  TextLayer *textLayer = (TextLayer *)dict_get(textLayersDict, nodeId);
  Layer *layer = text_layer_get_layer(textLayer);

  layer_remove_from_parent(layer);
  layer_destroy(layer);
  dict_remove(textLayersDict, nodeId);
}

// Public

void text_layer_reconciler_init()
{
  textLayersDict = dict_new();
}

void text_layer_reconciler_deinit()
{
  dict_free(textLayersDict);
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
    appendChild(parentLayer, nodeType, nodeId, propsDict);
    break;
  case OPERATION_COMMIT_UPDATE:
    commitUpdate(nodeType, nodeId, propsDict);
    break;
  case OPERATION_REMOVE_CHILD:
    removeChild(nodeType, nodeId);
    break;
  default:
    break;
  }
}