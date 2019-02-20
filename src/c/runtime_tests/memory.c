#include "memory.h"

#include "../utils/string.h"
#include "../utils/json_utils.h"

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

static char *type_labels[] = {"JSP_VALUE_UNKNOWN", "JSP_VALUE_STRING", "JSP_VALUE_OBJECT", "JSP_VALUE_ARRAY", "JSP_VALUE_PRIMITIVE"};

// PebbleDictionary *jsonProps;

// static void parsePropsJSONObject(JSP_ValueType type, char *label, uint16_t label_length, char *value, uint16_t value_length)
// {
//     char *l = calloc(label_length + 1, sizeof(char));
//     char *v = calloc(value_length + 1, sizeof(char));

//     snprintf(l, label_length + 1, "%s", label);
//     snprintf(v, value_length + 1, "%s", value);

//     if (strcmp(type_labels[type], "JSP_VALUE_STRING") == 0)
//     {
//         char *s = calloc(value_length - 1, sizeof(char));
//         char *sub = substr(v, 1, value_length - 1);
//         snprintf(s, value_length - 1, "%s", sub);
//         free(sub);
//         dict_add(jsonProps, l, s);
//     }

//     free(v);
//     free(l);
// }

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