#include "memory.h"

#include "../utils/string.h"
#include "../utils/json_utils.h"

#include "../reconcilers/constants.h"
#include "../reconcilers/text_layer.h"

static void logResult(const char *name, int before, int after)
{
    if (after > before)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "== Memory Tests == [%s] Error: There were %d bytes left in memory.", name, after - before);
    }
    else
    {
        APP_LOG(APP_LOG_LEVEL_INFO, "== Memory Tests == [%s] SUCCESS -", name);
    }
}

void assert_dict_add_remove()
{
    PebbleDictionary *dictionary = dict_new();
    int usedBefore = heap_bytes_used();
    dict_add(dictionary, "a", "avalue");
    dict_add(dictionary, "b", "bvalue");
    dict_add(dictionary, "c", "cvalue");
    dict_remove(dictionary, "a");
    dict_remove(dictionary, "b");
    dict_remove(dictionary, "c");
    logResult("assert_dict_add_remove", usedBefore, heap_bytes_used());
}

void assert_dict_new_free()
{
    int usedBefore = heap_bytes_used();
    PebbleDictionary *dictionary = dict_new();
    dict_add(dictionary, "a", "avalue");
    dict_add(dictionary, "b", "bvalue");
    dict_add(dictionary, "c", "cvalue");
    dict_remove(dictionary, "a");
    dict_remove(dictionary, "b");
    dict_remove(dictionary, "c");
    dict_free(dictionary);
    logResult("assert_dict_new_free", usedBefore, heap_bytes_used());
}

void assert_substr() {
    const char *v = "-this is a quite long string-";
    uint16_t length = strlen(v);
    char *s = calloc(length - 1, sizeof(char));
    int usedBefore = heap_bytes_used();
    char *sub = substr(v, 1, length - 1);
    snprintf(s, length - 1, "%s", sub);
    free(sub);
    logResult("assert_substr", usedBefore, heap_bytes_used());
    free(s);
}

void assert_json_object_parse()
{
    PebbleDictionary *jsonProps = dict_new();
    int usedBefore = heap_bytes_used();
    json_utils_parse_object_to_dict("{\"a\": \"avalue\", \"b\": \"bvalue\", \"c\": \"cvalue\"}", jsonProps);
    free(dict_get(jsonProps, "a"));
    dict_remove(jsonProps, "a");
    free(dict_get(jsonProps, "b"));
    dict_remove(jsonProps, "b");
    free(dict_get(jsonProps, "c"));
    dict_remove(jsonProps, "c");
    logResult("assert_json_object_parse", usedBefore, heap_bytes_used());
    dict_free(jsonProps);
}

void assert_json_array_parse()
{
    char **array = calloc(2, sizeof(char *));
    int usedBefore = heap_bytes_used();
    json_utils_parse_array_to_array("[{\"a\": \"avalue\", \"b\": \"bvalue\", \"c\": \"cvalue\"}, \"other\"]", array);
    free(array[0]);
    free(array[1]);
    logResult("assert_json_array_parse", usedBefore, heap_bytes_used());
    free(array);
}

void assert_text_reconciler_init_deinit()
{
    int usedBefore = heap_bytes_used();
    text_layer_reconciler_init();
    text_layer_reconciler_deinit();
    logResult("assert_text_reconciler_init_deinit", usedBefore, heap_bytes_used());
}

void assert_text_reconciler_add_remove(Layer *parentLayer)
{
    text_layer_reconciler_init();
    TextLayerPropsMessage textLayerProps = TextLayerPropsMessage_init_default;
    int usedBefore = heap_bytes_used();
    text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test", &textLayerProps);
    text_layer_reconciler(parentLayer, OPERATION_REMOVE_CHILD, NODE_TYPE_TEXT_LAYER, "test", NULL);
    logResult("assert_text_reconciler_add_remove", usedBefore, heap_bytes_used());
    text_layer_reconciler_deinit();
}

void assert_text_reconciler_remove_leftovers(Layer *parentLayer)
{
    TextLayerPropsMessage textLayerProps = TextLayerPropsMessage_init_default;
    int usedBefore = heap_bytes_used();
    text_layer_reconciler_init();
    text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test", &textLayerProps);
    text_layer_reconciler_deinit();
    logResult("assert_text_reconciler_remove_leftovers", usedBefore, heap_bytes_used());
}