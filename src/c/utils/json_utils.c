#include <pebble.h>

#include "json_utils.h"
#include "string.h"
#include "../lib/json/jsonparser.h"
#include "../lib/dictionary/dictionary.h"

static char *type_labels[] = {"JSP_VALUE_UNKNOWN", "JSP_VALUE_STRING", "JSP_VALUE_OBJECT", "JSP_VALUE_ARRAY", "JSP_VALUE_PRIMITIVE"};

static PebbleDictionary *currentDictionary;
static char **currentArray;
static int currentArrayIndex;

static void parseJSONObject(JSP_ValueType type, char *label, uint16_t label_length, char *value, uint16_t value_length)
{
    char *l = calloc(label_length + 1, sizeof(char));
    char *v = calloc(value_length + 1, sizeof(char));

    snprintf(l, label_length + 1, "%s", label);
    snprintf(v, value_length + 1, "%s", value);

    if (strcmp(type_labels[type], "JSP_VALUE_STRING") == 0)
    {
        dict_add(currentDictionary, l, substr(v, 1, value_length - 1));
        free(v);
    }
    else
    {
        dict_add(currentDictionary, l, v);
    }

    free(l);
}

void parseJSONArray(JSP_ValueType type, char *value, uint16_t value_length)
{
    char *v = calloc(value_length + 1, sizeof(char));
    snprintf(v, value_length + 1, "%s", value);

    if (strcmp(type_labels[type], "JSP_VALUE_OBJECT") == 0)
    {
        currentArray[currentArrayIndex++] = v;
    }
    else if (strcmp(type_labels[type], "JSP_VALUE_STRING") == 0)
    {
        currentArray[currentArrayIndex++] = substr(v, 1, value_length - 1);
        free(v);
    }
}

void json_utils_parse_object_to_dict(char *json, PebbleDictionary *dict)
{
    currentDictionary = dict;
    json_register_callbacks(parseJSONObject, NULL);
    json_parser(json);
}

void json_utils_parse_array_to_array(char *json, char **array)
{
    currentArray = array;
    currentArrayIndex = 0;
    json_register_callbacks(NULL, parseJSONArray);
    json_parser(json);
}