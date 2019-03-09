#include "animation_registry.h"
#include "../lib/dictionary/dictionary.h"

static PebbleDictionary *registry;

static char *getKey(uint8_t nodeType) {
    char *key = calloc(strlen("nodeType_") + 2, sizeof(char));

    snprintf(key, strlen("nodeType_") + 2, "nodeType_%d", nodeType);

    return key;
}

void animation_registry_add_callback(uint8_t nodeType, void *callback)
{
    int usedBefore = heap_bytes_used();

    char *key = getKey(nodeType);

    dict_add(registry, key, callback);
    free(key);

    APP_LOG(APP_LOG_LEVEL_ERROR, "Adding [%d] allocates %d bytes", nodeType, heap_bytes_used() - usedBefore);
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
    int usedBefore = heap_bytes_used();

    char *key = getKey(nodeType);

    dict_remove(registry, key);

    free(key);

    APP_LOG(APP_LOG_LEVEL_ERROR, "Removing [%d] saves %d bytes", nodeType, usedBefore - heap_bytes_used());
}

void animation_registry_init() {
    registry = dict_new();
}

void animation_registry_deinit() {
    dict_free(registry);
}