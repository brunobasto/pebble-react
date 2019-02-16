#include "layer_reconciler_utils.h"

void set_layer_frame_from_props(Layer *layer, PebbleDictionary *propsDict)
{
    GRect frame = layer_get_frame(layer);

    if (dict_has(propsDict, "top"))
    {
        uint16_t top = atoi((char *)dict_get(propsDict, "top"));
        frame.origin.y = top;
    }
    if (dict_has(propsDict, "left"))
    {
        uint16_t left = atoi((char *)dict_get(propsDict, "left"));
        frame.origin.x = left;
    }
    if (dict_has(propsDict, "width"))
    {
        uint16_t width = atoi((char *)dict_get(propsDict, "width"));
        frame.size.w = width;
    }
    if (dict_has(propsDict, "height"))
    {
        uint16_t height = atoi((char *)dict_get(propsDict, "height"));
        frame.size.h = height;
    }
    layer_set_frame(layer, GRect(frame.origin.x, frame.origin.y, frame.size.w, frame.size.h));
}

GRect get_layer_frame_from_props(PebbleDictionary *propsDict)
{
    uint16_t top = atoi((char *)dict_get(propsDict, "top"));
    uint16_t left = atoi((char *)dict_get(propsDict, "left"));
    uint16_t width = atoi((char *)dict_get(propsDict, "width"));
    uint16_t height = atoi((char *)dict_get(propsDict, "height"));

    return GRect(left, top, width, height);
}