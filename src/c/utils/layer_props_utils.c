#include "layer_props_utils.h"
#include "../lib/dictionary/dictionary.h"

void layer_props_utils_merge_props(PebbleDictionary *dest, PebbleDictionary *source)
{
    PebbleDictionary *temp = source;

    if (temp == NULL)
    {
        return;
    }
    if (temp->head == NULL)
    {
        return;
    }
    while (temp != NULL)
    {
        dict_add(dest, temp->head->key, temp->head->value);
        temp = temp->tail;
    }
}

void layer_props_utils_set_frame_from_props(Layer *layer, PebbleDictionary *propsDict)
{
    GRect frame = layer_get_frame(layer);

    if (dict_has(propsDict, "top"))
    {
        frame.origin.y = atoi((char *)dict_get(propsDict, "top"));
    }
    if (dict_has(propsDict, "left"))
    {
        frame.origin.x = atoi((char *)dict_get(propsDict, "left"));
    }
    if (dict_has(propsDict, "width"))
    {
        frame.size.w = atoi((char *)dict_get(propsDict, "width"));
    }
    if (dict_has(propsDict, "height"))
    {
        frame.size.h = atoi((char *)dict_get(propsDict, "height"));
    }
    layer_set_frame(layer, frame);
}

GRect layer_props_utils_get_frame_from_props(PebbleDictionary *propsDict)
{
    uint16_t top = atoi((char *)dict_get(propsDict, "top"));
    uint16_t left = atoi((char *)dict_get(propsDict, "left"));
    uint16_t width = atoi((char *)dict_get(propsDict, "width"));
    uint16_t height = atoi((char *)dict_get(propsDict, "height"));

    return GRect(left, top, width, height);
}