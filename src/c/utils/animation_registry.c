#include "animation_registry.h"
#include "../lib/dictionary/dictionary.h"

static PebbleDictionary *registry;

static char *getKey(uint8_t nodeType) {
    char *key = calloc(4, sizeof(char));

    snprintf(key, 4, "%d", nodeType);

    return key;
}

void animation_registry_add_callback(uint8_t nodeType, void *callback)
{
    char *key = getKey(nodeType);

    dict_add(registry, key, callback);
    free(key);
}

void *animation_registry_get_callback(uint8_t nodeType)
{
    char *key = getKey(nodeType);
    void *callback = dict_get(registry, key);

    free(key);

    return callback;
}

void animation_registry_remove_callback(uint8_t nodeType)
{
    char *key = getKey(nodeType);

    dict_remove(registry, key);
    free(key);
}

void animation_registry_init() {
    registry = dict_new();
}

void animation_registry_deinit() {
    dict_free(registry);
}