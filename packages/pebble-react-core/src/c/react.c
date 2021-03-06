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
#include "./utils/layers_registry.h"
#include "./utils/operations.h"
#include "./utils/string.h"

// Reconcilers
#include "./reconcilers/animation.h"
#include "./reconcilers/arc_layer.h"
#include "./reconcilers/circle_layer.h"
#include "./reconcilers/layer.h"
#include "./reconcilers/path_layer.h"
#include "./reconcilers/text_layer.h"

static Window *mainWindow;

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

static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // TODO
}

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  // TODO
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

    pb_istream_t stream = pb_istream_from_buffer(buffer, batchOperationsByteLength);

    bool status = pb_decode(&stream, BatchOperationsMessage_fields, &batchOperationsMessage);

    if (!status)
    {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
      return;
    }

    operations_process_batch(mainWindow, &batchOperationsMessage);
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
  window_set_background_color(window, GColorBlack);
}

static void prv_window_unload(Window *window)
{
}

static void appInit(void)
{
  // <Window>
  window_set_click_config_provider(mainWindow, prv_click_config_provider);
  window_set_window_handlers(
      mainWindow,
      (WindowHandlers){
          .load = prv_window_load,
          .unload = prv_window_unload,
      });

  const bool animated = true;
  window_stack_push(mainWindow, animated);
  // </Window>

  animation_registry_init();
  layer_registry_init();
  layer_reconciler_init();

  animation_reconciler_init();
  arc_layer_reconciler_init();
  circle_layer_reconciler_init();
  path_layer_reconciler_init();
  text_layer_reconciler_init();

  configureAppMessage();

  APP_LOG(APP_LOG_LEVEL_INFO, "after appInit we use %d", heap_bytes_used());
}

static void appDeinit(void)
{
  animation_reconciler_deinit();
  arc_layer_reconciler_deinit();
  circle_layer_reconciler_deinit();
  path_layer_reconciler_deinit();
  text_layer_reconciler_deinit();

  layer_reconciler_deinit();
  animation_registry_deinit();
  layer_registry_deinit();

  window_stack_pop_all(false);
  window_destroy(mainWindow);
}

int main(void)
{
  mainWindow = window_create();

  appInit();
  app_event_loop();
  appDeinit();
}