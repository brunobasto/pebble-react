#include "image_layer.h"
#include "../utils/layer_props_utils.h"
#include "../utils/layers_registry.h"

static PebbleDictionary *imageLayersDict;

static void setImageLayerProperties(Layer *imageLayer, PebbleDictionary *propsDict)
{
  if (propsDict == NULL)
  {
    return;
  }

  layer_props_utils_set_frame_from_props(imageLayer, propsDict);
}

static void appendChild(
    Layer *parentLayer,
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  // Create bitmap
  GBitmap *imageBitmap = gbitmap_create_with_resource(RESOURCE_ID_STATIC);
  // Creates a rotateable bitmap layer
  RotBitmapLayer *imageRotBitmap = rot_bitmap_layer_create(imageBitmap);
  rot_bitmap_layer_set_angle(imageRotBitmap, DEG_TO_TRIGANGLE(0));
  rot_bitmap_set_compositing_mode(imageRotBitmap, GCompOpSet);
  // rot_bitmap_set_src_ic(imageRotBitmap, GPoint(0, 0));

  // Create layer
  Layer *imageLayer = bitmap_layer_get_layer((BitmapLayer *)imageRotBitmap);
  layer_add_child(parentLayer, imageLayer);

  dict_add(imageLayersDict, nodeId, imageLayer);
  setImageLayerProperties(imageLayer, propsDict);
}

static void commitUpdate(
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  Layer *imageLayer = (Layer *)dict_get(imageLayersDict, nodeId);

  setImageLayerProperties(imageLayer, propsDict);
}

static void removeChild(
    const uint16_t nodeType,
    const char *nodeId)
{
  Layer *imageLayer = (Layer *)dict_get(imageLayersDict, nodeId);

  layer_remove_from_parent(imageLayer);
  layer_destroy(imageLayer);
  dict_remove(imageLayersDict, nodeId);
}

// Public

void image_layer_reconciler_init()
{
  imageLayersDict = dict_new();
}

void image_layer_reconciler_deinit()
{
  dict_free(imageLayersDict);
}

void image_layer_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *propsDict)
{
  if (nodeType != NODE_TYPE_IMAGE_LAYER)
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