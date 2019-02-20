#include "layers_registry.h"
#include "../lib/hashmap/hashmap.h"

static PebbleDictionary *layersRegistryDictionary;
static PebbleHashMap *layersReconcilersRegistryHashMap;

char *layer_registry_get_node_id(Layer *layer)
{
    PebbleDictionary *temp = layersRegistryDictionary;

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

Layer *layer_registry_get(const char *nodeId)
{
    return dict_get(layersRegistryDictionary, nodeId);
}

int layer_registry_has(const char *nodeId)
{
    return dict_has(layersRegistryDictionary, nodeId);
}

void *layer_registry_get_reconciler(Layer *layer)
{
    return hash_get(layersReconcilersRegistryHashMap, layer);
}

void layer_registry_remove_reconciler(Layer *layer)
{
    hash_remove(layersReconcilersRegistryHashMap, layer);
}

void layer_registry_add(const char *nodeId, Layer *layer)
{
    dict_add(layersRegistryDictionary, nodeId, layer);
}

void layer_registry_add_reconciler(Layer *layer, void *reconciler)
{
    hash_add(layersReconcilersRegistryHashMap, layer, reconciler);
}

void layer_registry_remove(const char *nodeId)
{
    dict_remove(layersRegistryDictionary, nodeId);
}

void layer_registry_init()
{
    layersRegistryDictionary = dict_new();
    layersReconcilersRegistryHashMap = hash_new();
}

void layer_registry_deinit()
{
    dict_free(layersRegistryDictionary);
    hash_free(layersReconcilersRegistryHashMap);
}