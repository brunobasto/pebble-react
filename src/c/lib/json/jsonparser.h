#pragma once

/*
 * MIT License http://dogan.mit-license.org/
 */

typedef enum JSP_ErrorType
{
  JSP_OK = 0x0,                                   // OK
  JSP_EMPTY = 0x1,                                // Empty value
  JSP_ERROR_NULL_INPUT = 0x101,                   // The NULL pointer passed to function
  JSP_ERROR_NOT_JSON = 0x202,                     // The JSON needs to start with { or [
  JSP_ERROR_NOT_STRING = 0x203,                   // The string needs to start with "
  JSP_ERROR_NOT_OBJECT = 0x204,                   // The object needs to start with {
  JSP_ERROR_NOT_ARRAY = 0x205,                    // The array needs to start with [
  JSP_ERROR_NOT_FOUND = 0x206,                    // The colon, semicolon or beggining of object or array not found
  JSP_ERROR_UNEXPECTED_QUOTE = 0x301,             // The primitive should not include "
  JSP_ERROR_UNEXPECTED_CHAR_IN_PRIMITIVE = 0x302, // The primitive needs to be a number
  JSP_ERROR_UNEXPECTED_CHAR_IN_OBJECT = 0x303,    // The object needs to be in format {label:value,label:value}
  JSP_ERROR_UNEXPECTED_CHAR_IN_ARRAY = 0x304,     // The array needs to be in format [value,value,value]
  JSP_ERROR_UNEXPECTED_END = 0x401,               // The json finished too early
} JSP_ErrorType;

typedef enum JSP_ValueType
{
  JSP_VALUE_UNKNOWN = 0,
  JSP_VALUE_STRING = 1,
  JSP_VALUE_OBJECT = 2,
  JSP_VALUE_ARRAY = 3,
  JSP_VALUE_PRIMITIVE = 4,
} JSP_ValueType;

/*
 * The parser
 *
 * One argument is the pointer to the beginning of null terminated json string
 *
 * If there is no callback initialized, the json is parsed and if it has
 * no errors, JSP_OK is returned
 *
 * If callback is registered, it is called on each value of array or object
 *
 * The callback function needs to check the type of value and process it, e.g.:

char* type_labels[] = {"JSP_VALUE_UNKNOWN", "JSP_VALUE_STRING", "JSP_VALUE_OBJECT", "JSP_VALUE_ARRAY", "JSP_VALUE_PRIMITIVE"};

void object_callback(JSP_ValueType type, char* label, uint16_t label_length, char* value, uint16_t value_length) {
  char* l = calloc(label_length + 1, sizeof(char));
  char* v = calloc(value_length + 1, sizeof(char));

  snprintf(l, label_length + 1, "%s", label);
  snprintf(v, value_length + 1, "%s", value);

  APP_LOG(APP_LOG_LEVEL_INFO, "object_callback(%s, %s, %d, %s, %d)", type_labels[type], l, label_length, v, value_length);

  free(v);
  free(l);
}

void array_callback(JSP_ValueType type, char* value, uint16_t value_length) {
  char* v = calloc(value_length + 1, sizeof(char));

  snprintf(v, value_length + 1, "%s", value);

  APP_LOG(APP_LOG_LEVEL_INFO, "array_callback(%s, %s, %d)", type_labels[type], v, value_length);

  free(v);
}


 */

JSP_ErrorType json_parser(const char *json_in);

/*
 * The call back type (see example above)
 */

typedef void (*JSP_ObjectCallback)(JSP_ValueType type, char *label, uint16_t label_length, char *value, uint16_t value_length);
typedef void (*JSP_ArrayCallback)(JSP_ValueType type, char *value, uint16_t value_length);

/*
 * Callback registration
 *
 * Two argument are the callback functions or null if the callback should be deregistered
 */

JSP_ErrorType json_register_callbacks(JSP_ObjectCallback object_callback, JSP_ArrayCallback array_callback);
