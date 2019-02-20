#include <pebble.h>

// Tests
#include "./runtime_tests/memory.h"

// Libs
#include "./lib/json/jsonparser.h"
#include "./lib/dictionary/dictionary.h"

// Utils
#include "./utils/string.h"
#include "./utils/json_utils.h"

// Reconcilers
#include "./reconcilers/animation.h"
#include "./reconcilers/image_layer.h"
#include "./reconcilers/text_layer.h"
#include "./utils/layers_registry.h"

AppTimer *timer;

static Window *s_window;

static void handleOperation(PebbleDictionary *operationDict)
{
  if (dict_has(operationDict, "operation") != 1)
  {
    return;
  }

  // Operation
  const uint16_t operation = atoi(dict_get(operationDict, "operation"));
  // APP_LOG(APP_LOG_LEVEL_INFO, "handling operation %d", operation);

  // Node Type
  const uint16_t nodeType = atoi(dict_get(operationDict, "nodeType"));

  // Node Id
  const char *nodeId = dict_get(operationDict, "nodeId");

  // Props
  PebbleDictionary *propsDict = NULL;

  if (dict_has(operationDict, "props"))
  {
    propsDict = dict_new();

    char *propsJSON = dict_get(operationDict, "props");

    APP_LOG(APP_LOG_LEVEL_INFO, "before parse props json %d", heap_bytes_used());

    json_utils_parse_object_to_dict(propsJSON, propsDict);

    APP_LOG(APP_LOG_LEVEL_INFO, "after parse props json %d", heap_bytes_used());
  }

  Layer *window_layer = window_get_root_layer(s_window);

  // Reconcilers
  text_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
  image_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
  animation_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
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

    tuple = dict_find(received, MESSAGE_KEY_batchOperationsLength);

    uint16_t batchOperationsLength = tuple->value->uint16;

    char **batchOperations = calloc(batchOperationsLength, sizeof(char *));

    json_utils_parse_array_to_array(batchOperationsJSON, batchOperations);

    for (uint16_t i = 0; i < batchOperationsLength; i++)
    {
      PebbleDictionary *operationDict = dict_new();

      json_utils_parse_object_to_dict(batchOperations[i], operationDict);

      APP_LOG(APP_LOG_LEVEL_INFO, "before operation we use %d", heap_bytes_used());

      handleOperation(operationDict);

      APP_LOG(APP_LOG_LEVEL_INFO, "after operation we use %d", heap_bytes_used());

      dict_free(operationDict);

      free(batchOperations[i]);
    }

    free(batchOperations);
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
}

static void prv_window_unload(Window *window)
{
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

  layer_registry_init();

  text_layer_reconciler_init();
  image_layer_reconciler_init();
  animation_reconciler_init();

  APP_LOG(APP_LOG_LEVEL_INFO, "after prv_init we use %d", heap_bytes_used());
}

static void prv_deinit(void)
{
  text_layer_reconciler_deinit();
  image_layer_reconciler_deinit();
  animation_reconciler_deinit();

  layer_registry_deinit();

  window_destroy(s_window);
}

int main(void)
{
  // assert_dict_add_remove();
  // assert_dict_new_free();
  // assert_json_object_parse();
  // assert_json_array_parse();
  // assert_substr();
  APP_LOG(APP_LOG_LEVEL_INFO, "Currently using %d", heap_bytes_used());
  prv_init();
  app_event_loop();
  prv_deinit();
}
