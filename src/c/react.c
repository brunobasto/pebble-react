#include <pebble.h>

// Libs
#include "./lib/pebble-c-jsonparser/jsonparser.h"
#include "./lib/dictionary/dictionary.h"

// Utils
#include "./utils/string.h"

// Reconcilers
#include "./reconcilers/text_layer.h"
#include "./reconcilers/image_layer.h"

AppTimer *timer;

static Window *s_window;

PebbleDictionary *operationDict;
PebbleDictionary *propsDict;

#define MAX_BATCH_OPERATIONS 100
char *batchOperations[MAX_BATCH_OPERATIONS];
uint16_t batchOperationsCounter = 0;
// TODO - do it dynamically
// char **orderedIds;

// orderedIds = malloc(variableNumberOfElements * sizeof(char*));
// for (int i = 0; i < variableNumberOfElements; i++)
//     orderedIds[i] = malloc((ID_LEN+1) * sizeof(char));

char *type_labels[] = {"JSP_VALUE_UNKNOWN", "JSP_VALUE_STRING", "JSP_VALUE_OBJECT", "JSP_VALUE_ARRAY", "JSP_VALUE_PRIMITIVE"};

void parsePropsJSONObject(JSP_ValueType type, char *label, uint16_t label_length, char *value, uint16_t value_length)
{
  char *l = calloc(label_length + 1, sizeof(char));
  char *v = calloc(value_length + 1, sizeof(char));
  char *s = calloc(value_length - 1, sizeof(char));

  snprintf(l, label_length + 1, "%s", label);
  snprintf(v, value_length + 1, "%s", value);
  // APP_LOG(APP_LOG_LEVEL_INFO, "parse prop value(%s, %s, %s)", type_labels[type], l, v);
  snprintf(s, value_length + 1, "%s", substr(v, 1, value_length - 1));
  dict_add(propsDict, l, s);
  free(v);
  free(l);
}

static void handleOperation()
{
  // Operation
  const uint16_t operation = atoi(dict_get(operationDict, "operation"));
  // APP_LOG(APP_LOG_LEVEL_INFO, "handling operation %d", operation);

  // Node Type
  const uint16_t nodeType = atoi(dict_get(operationDict, "nodeType"));

  // Node Id
  const char *nodeId = dict_get(operationDict, "nodeId");

  // Props
  propsDict = NULL;

  if (dict_has(operationDict, "props"))
  {
    propsDict = dict_new();

    const char *propsJSON = dict_get(operationDict, "props");

    json_register_callbacks(parsePropsJSONObject, NULL);

    json_parser(propsJSON);
  }

  Layer *window_layer = window_get_root_layer(s_window);

  // Reconcilers
  text_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
  image_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);

  dict_free(propsDict);
}

void parseBatchOperationsJSONObject(JSP_ValueType type, char *label, uint16_t label_length, char *value, uint16_t value_length)
{
  // APP_LOG(APP_LOG_LEVEL_INFO, "type of object value(%s, %s, %s)", type_labels[type], label, value);

  char *l = calloc(label_length + 1, sizeof(char));
  char *v = calloc(value_length + 1, sizeof(char));

  snprintf(l, label_length + 1, "%s", label);
  snprintf(v, value_length + 1, "%s", value);

  if (strcmp(type_labels[type], "JSP_VALUE_OBJECT") == 0)
  {
    dict_add(operationDict, l, v);
  }
  else
  {
    char *s = calloc(value_length - 1, sizeof(char));
    snprintf(s, value_length + 1, "%s", substr(v, 1, value_length - 1));
    dict_add(operationDict, l, s);
    free(v);
  }

  free(l);
}

void parseBatchOperationsJSONArray(JSP_ValueType type, char *value, uint16_t value_length)
{
  char *v = calloc(value_length + 1, sizeof(char));
  snprintf(v, value_length + 1, "%s", value);

  if (strcmp(type_labels[type], "JSP_VALUE_OBJECT") == 0)
  {
    batchOperations[batchOperationsCounter++] = v;
  }
}

static void handleAnimationTimer(void *context)
{
  // uint32_t next_delay = 10;

  // rot_bitmap_layer_increment_angle(icon_rot_bitmap_layer, DEG_TO_TRIGANGLE(15));

  // timer = app_timer_register(next_delay, handleAnimationTimer, NULL);
}

static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // text_layer_set_text(s_text_layer, "Select");
}

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // text_layer_set_text(s_text_layer, "Up");
}

static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // text_layer_set_text(s_text_layer, "Down");
}

static void prv_click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
}

static void addIconLayer(Layer *window_layer)
{
  // timer = app_timer_register(1, handleAnimationTimer, NULL);
}

static void handleMessageDropped(AppMessageResult reason, void *context)
{
}

static void handleMessageOutFailed(DictionaryIterator *failed, AppMessageResult reason, void *context)
{
}

static void handleMessageReceived(DictionaryIterator *received, void *context)
{
  Tuple *tuple;

  // Batch Operations
  tuple = dict_find(received, MESSAGE_KEY_batchOperations);

  if (tuple)
  {
    char *batchOperationsJSON = tuple->value->cstring;

    json_register_callbacks(NULL, parseBatchOperationsJSONArray);

    json_parser(batchOperationsJSON);

    for (uint16_t i = 0; i < batchOperationsCounter; i++)
    {
      operationDict = dict_new();

      json_register_callbacks(parseBatchOperationsJSONObject, NULL);

      JSP_ErrorType error = json_parser(batchOperations[i]);

      if (error == JSP_OK)
      {
        handleOperation();
      }

      dict_free(operationDict);

      free(batchOperations[i]);
    }

    batchOperationsCounter = 0;
  }
}

static void configureAppMessage()
{
  // Register AppMessage handlers
  app_message_register_inbox_received(handleMessageReceived);
  app_message_register_inbox_dropped(handleMessageDropped);
  app_message_register_outbox_failed(handleMessageOutFailed);

  // Initialize AppMessage inbox and outbox buffers with a suitable size
  const int inbox_size = 8200;
  const int outbox_size = 8200;
  app_message_open(inbox_size, outbox_size);
}

static void prv_window_load(Window *window)
{
  window_set_background_color(window, GColorRed);
  // Layer *window_layer = window_get_root_layer(window);
  // addIconLayer(window_layer);
}

static void prv_window_unload(Window *window)
{
  // text_layer_destroy(s_text_layer);
}

static void prv_init(void)
{
  configureAppMessage();

  // <Window>
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(
      s_window,
      (WindowHandlers){
          .load = prv_window_load,
          .unload = prv_window_unload,
      });

  const bool animated = true;
  window_stack_push(s_window, animated);
  // </Window>

  text_layer_reconciler_init();
  image_layer_reconciler_init();
}

static void prv_deinit(void)
{
  text_layer_reconciler_deinit();
  image_layer_reconciler_deinit();

  window_destroy(s_window);
}

int main(void)
{
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
