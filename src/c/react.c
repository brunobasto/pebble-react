#include <pebble.h>

// Tests
#include "./runtime_tests/memory.h"

// Libs
// #include "./lib/json/jsonparser.h"
#include "./lib/dictionary/dictionary.h"

// Protocol Buffers
#include "./lib/nanopb/pb_encode.h"
#include "./lib/nanopb/pb_decode.h"
#include "./lib/nanopb/BatchOperationsMessage.pb.h"

// Utils
#include "./utils/animation_registry.h"
#include "./utils/json_utils.h"
#include "./utils/layers_registry.h"
#include "./utils/string.h"

// Reconcilers
#include "./reconcilers/constants.h"
#include "./reconcilers/animation.h"
#include "./reconcilers/image_layer.h"
#include "./reconcilers/text_layer.h"

AppTimer *timer;

static Window *s_window;

static void sendMessage(void *context, char *message)
{
  DictionaryIterator *iter;

  uint32_t result = app_message_outbox_begin(&iter);

  if (result == APP_MSG_OK)
  {
    dict_write_cstring(iter, MESSAGE_KEY_message, message);
    dict_write_end(iter);
    app_message_outbox_send();
  }
}

static void handleOperation(OperationMessage *operationMessage)
{
  const uint8_t operation = operationMessage->operation;
  const uint8_t nodeType = operationMessage->nodeType;
  const char *nodeId = operationMessage->nodeId;

  Layer *window_layer = window_get_root_layer(s_window);

  switch (nodeType)
  {
  case NODE_TYPE_TEXT_LAYER:
  {
    TextLayerPropsMessage *textLayerProps = operationMessage->textLayerProps;

    text_layer_reconciler(window_layer, operation, nodeType, nodeId, textLayerProps);
  }
  break;

  default:
    break;
  }

  // // Reconcilers
  // text_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
  // image_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
  // animation_reconciler(window_layer, operation, nodeType, nodeId, propsDict);
}

static void handleBatchOperations(BatchOperationsMessage *batchOperations)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "got %d operations", batchOperations->operations_count);

  for (uint16_t i = 0; i < batchOperations->operations_count; i++)
  {
    OperationMessage operationMessage = batchOperations->operations[i];

    handleOperation(&operationMessage);
  }

  free(batchOperations->operations);
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
  sendMessage(NULL, "down");
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

  tuple = dict_find(received, MESSAGE_KEY_batchOperations);

  if (tuple)
  {
    uint8_t *buffer = tuple->value->data;

    tuple = dict_find(received, MESSAGE_KEY_batchOperationsByteLength);

    uint16_t batchOperationsByteLength = tuple->value->uint16;

    // Decode
    BatchOperationsMessage batchOperationsMessage = BatchOperationsMessage_init_zero;

    // batchOperationsMessage.funcs.decode

    pb_istream_t stream = pb_istream_from_buffer(buffer, batchOperationsByteLength);

    bool status = pb_decode(&stream, BatchOperationsMessage_fields, &batchOperationsMessage);

    if (!status)
    {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
      return;
    }

    handleBatchOperations(&batchOperationsMessage);
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

  // Reconcilers Tests
  Layer *window_layer = window_get_root_layer(s_window);

  animation_registry_init();
  layer_registry_init();

  // assert_text_reconciler_init_deinit();
  // assert_text_reconciler_add_remove(window_layer);
  // assert_text_reconciler_remove_leftovers(window_layer);

  text_layer_reconciler_init();
  image_layer_reconciler_init();
  animation_reconciler_init();

  configureAppMessage();

  APP_LOG(APP_LOG_LEVEL_INFO, "after prv_init we use %d", heap_bytes_used());
}

static void prv_deinit(void)
{
  text_layer_reconciler_deinit();
  image_layer_reconciler_deinit();
  animation_reconciler_deinit();

  animation_registry_deinit();
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