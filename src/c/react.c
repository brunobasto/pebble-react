#include <pebble.h>

// Libs
#include "./lib/pebble-c-jsonparser/jsonparser.h"
#include "./lib/dictionary/dictionary.h"

// Utils
#include "./utils/string.h"

// Reconcilers
#include "./reconcilers/text_layer.h"

// Spinner
static GBitmapSequence *spinner_sequence;
static GBitmap *spinner_bitmap;
static BitmapLayer *spinner_bitmap_layer;
AppTimer *timer;

// Icon
static BitmapLayer *icon_bitmap_layer;
static GBitmap *icon_bitmap;
static RotBitmapLayer *icon_rot_bitmap_layer;

static Window *s_window;
static TextLayer *s_text_layer;

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
  // free(l);
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

    // APP_LOG(APP_LOG_LEVEL_INFO, "handling props %s", propsJSON);

    json_register_callbacks(parsePropsJSONObject, NULL);

    json_parser(propsJSON);
  }

  Layer *window_layer = window_get_root_layer(s_window);

  // APP_LOG(APP_LOG_LEVEL_INFO, "calling reconciler");

  // Reconcilers
  text_layer_reconciler(window_layer, operation, nodeType, nodeId, propsDict);

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

  // free(l);
}

void parseBatchOperationsJSONArray(JSP_ValueType type, char *value, uint16_t value_length)
{
  char *v = calloc(value_length + 1, sizeof(char));
  snprintf(v, value_length + 1, "%s", value);

  // APP_LOG(APP_LOG_LEVEL_INFO, "type of array value(%s, %s, %d)", type_labels[type], value, value_length);

  if (strcmp(type_labels[type], "JSP_VALUE_OBJECT") == 0)
  {
    // APP_LOG(APP_LOG_LEVEL_INFO, "objectInArray(%s)", v);

    batchOperations[batchOperationsCounter++] = v;
  }
}

static void runSpinnerSequence();

static void handleSpinnerTimer(void *context)
{
  uint32_t next_delay;

  // Advance to the next APNG frame
  if (gbitmap_sequence_update_bitmap_next_frame(spinner_sequence, spinner_bitmap, &next_delay))
  {
    bitmap_layer_set_bitmap(spinner_bitmap_layer, spinner_bitmap);
    layer_mark_dirty(bitmap_layer_get_layer(spinner_bitmap_layer));

    // Timer for that delay
    timer = app_timer_register(next_delay, handleSpinnerTimer, NULL);
  }
  else
  {
    // Start again
    runSpinnerSequence();
  }
}

static void handleAnimationTimer(void *context)
{
  uint32_t next_delay = 10;

  rot_bitmap_layer_increment_angle(icon_rot_bitmap_layer, DEG_TO_TRIGANGLE(15));

  timer = app_timer_register(next_delay, handleAnimationTimer, NULL);
}

static void runSpinnerSequence()
{
  // Free old data
  if (spinner_sequence)
  {
    gbitmap_sequence_destroy(spinner_sequence);
    spinner_sequence = NULL;
  }
  if (spinner_bitmap)
  {
    gbitmap_destroy(spinner_bitmap);
    spinner_bitmap = NULL;
  }

  // Create sequence
  spinner_sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_SPINNER);

  // Create GBitmap
  spinner_bitmap = gbitmap_create_blank(gbitmap_sequence_get_bitmap_size(spinner_sequence), GBitmapFormat8Bit);

  // Begin animation
  timer = app_timer_register(1, handleSpinnerTimer, NULL);
}

static void showLoading()
{
  if (layer_get_hidden((Layer *)spinner_bitmap_layer))
  {
    layer_set_hidden((Layer *)spinner_bitmap_layer, false);
    runSpinnerSequence();
  }
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

static void addSpinnerLayer(Layer *window_layer, GRect frame)
{
  spinner_bitmap_layer = bitmap_layer_create(GRect(10, 10, frame.size.w, frame.size.h));
  bitmap_layer_set_background_color(spinner_bitmap_layer, GColorWhite);
  layer_add_child(window_layer, bitmap_layer_get_layer(spinner_bitmap_layer));
  layer_set_hidden((Layer *)spinner_bitmap_layer, true);
}

static void icon_bitmap_layer_update_proc(Layer *bitmap_layer, GContext *ctx)
{
  GRect bounds = layer_get_bounds(bitmap_layer);
  GPoint centerPoint = GPoint(bounds.origin.x + bounds.size.w / 2, bounds.origin.y + bounds.size.h / 2);
  GPoint destPoint = GPoint(bounds.origin.x + bounds.size.w / 2, bounds.origin.y + bounds.size.h / 2);

  graphics_draw_rotated_bitmap(ctx, icon_bitmap, centerPoint, DEG_TO_TRIGANGLE(60), centerPoint);
}

static void addIconLayer(Layer *window_layer)
{
  // Create bitmap
  icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_STATIC);
  // Creates a rotateable bitmap layer
  icon_rot_bitmap_layer = rot_bitmap_layer_create(icon_bitmap);
  rot_bitmap_layer_set_angle(icon_rot_bitmap_layer, DEG_TO_TRIGANGLE(60));
  rot_bitmap_set_compositing_mode(icon_rot_bitmap_layer, GCompOpSet);
  // rot_bitmap_set_src_ic(icon_rot_bitmap_layer, GPoint(0, 0));

  // Create layer
  Layer *icon_layer = bitmap_layer_get_layer((BitmapLayer *)icon_rot_bitmap_layer);
  layer_add_child(window_layer, icon_layer);

  timer = app_timer_register(1, handleAnimationTimer, NULL);
}

static void handleMessageDropped(AppMessageResult reason, void *context)
{
}

static void handleMessageOutFailed(DictionaryIterator *failed, AppMessageResult reason, void *context)
{
}

static void handleMessageReceived(DictionaryIterator *received, void *context)
{
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "Got message");
  Layer *window_layer = window_get_root_layer(s_window);

  Tuple *tuple;

  // Batch Operations
  tuple = dict_find(received, MESSAGE_KEY_batchOperations);

  if (tuple)
  {
    char *batchOperationsJSON = tuple->value->cstring;

    json_register_callbacks(NULL, parseBatchOperationsJSONArray);

    // APP_LOG(APP_LOG_LEVEL_DEBUG, "JSON %d : %s", strlen(batchOperationsJSON), batchOperationsJSON);

    JSP_ErrorType error = json_parser(batchOperationsJSON);

    // APP_LOG(APP_LOG_LEVEL_DEBUG, "Result 0x%x\n", error);

    for (uint16_t i = 0; i < batchOperationsCounter; i++)
    {
      operationDict = dict_new();

      json_register_callbacks(parseBatchOperationsJSONObject, NULL);

      JSP_ErrorType error = json_parser(batchOperations[i]);

      // APP_LOG(APP_LOG_LEVEL_DEBUG, "Operation Result 0x%x\n", error);

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
  Layer *window_layer = window_get_root_layer(window);
  addIconLayer(window_layer);
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
}

static void prv_deinit(void)
{
  text_layer_reconciler_deinit();

  window_destroy(s_window);
}

int main(void)
{
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
