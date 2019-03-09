#include <pebble.h>

void animation_registry_add_callback(uint8_t nodeType, void *callback);
void animation_registry_deinit();
void *animation_registry_get_callback(uint8_t nodeType);
void animation_registry_init();
void animation_registry_remove_callback(uint8_t nodeType);