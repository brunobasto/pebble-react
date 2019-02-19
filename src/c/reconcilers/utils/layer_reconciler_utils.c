#include "layer_reconciler_utils.h"
#include "../../lib/dictionary/dictionary.h"
#include "../../lib/hashmap/hashmap.h"

static PebbleDictionary *layersDict;
static PebbleHashMap *reconcilersHashMap;

char *layer_registry_get_node_id(Layer *layer)
{
    PebbleDictionary *temp = layersDict;

    while (temp != NULL)
    {
        if (temp->head->value == layer)
        {
            return temp->head->key;
        }
        temp = temp->tail;
    }
    return 0;
}

void layer_utils_merge_props(PebbleDictionary *dest, PebbleDictionary *source)
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

void set_layer_frame_from_props(Layer *layer, PebbleDictionary *propsDict)
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

GRect get_layer_frame_from_props(PebbleDictionary *propsDict)
{
    uint16_t top = atoi((char *)dict_get(propsDict, "top"));
    uint16_t left = atoi((char *)dict_get(propsDict, "left"));
    uint16_t width = atoi((char *)dict_get(propsDict, "width"));
    uint16_t height = atoi((char *)dict_get(propsDict, "height"));

    return GRect(left, top, width, height);
}

Layer *layer_registry_get(const char *nodeId)
{
    return dict_get(layersDict, nodeId);
}

int layer_registry_has(const char *nodeId)
{
    return dict_has(layersDict, nodeId);
}

void *layer_registry_get_reconciler(Layer *layer)
{
    return hash_get(reconcilersHashMap, layer);
}

void layer_registry_remove_reconciler(Layer *layer)
{
    hash_remove(reconcilersHashMap, layer);
}

void layer_registry_add(const char *nodeId, Layer *layer)
{
    dict_add(layersDict, nodeId, layer);
}

void layer_registry_add_reconciler(Layer *layer, void *reconciler)
{
    hash_add(reconcilersHashMap, layer, reconciler);
}

void layer_registry_remove(const char *nodeId)
{
    dict_remove(layersDict, nodeId);
}

void layer_registry_init()
{
    layersDict = dict_new();
    reconcilersHashMap = hash_new();
}

void layer_registry_deinit()
{
    dict_free(layersDict);
    hash_free(reconcilersHashMap);
}