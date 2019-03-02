#include "memory.h"

#include "../utils/string.h"
#include "../utils/json_utils.h"
#include "../utils/operations.h"
#include "../utils/layers_registry.h"
#include "../utils/animation_registry.h"

#include "../reconcilers/constants.h"
#include "../reconcilers/animation.h"
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

/*
 * Dictionary memory leak tests
 */

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
    dict_free(dictionary);
}

static PebbleDictionary *foreachDict;
static uint8_t foreachDictCount = 0;

static void countForeachDictElement()
{
    foreachDictCount++;
}

void assert_dict_foreach_count()
{
    int usedBefore = heap_bytes_used();
    foreachDict = dict_new();
    dict_add(foreachDict, "a", "avalue");
    dict_add(foreachDict, "b", "bvalue");
    dict_add(foreachDict, "c", "cvalue");
    dict_foreach(foreachDict, countForeachDictElement);
    if (foreachDictCount != 3)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "== Memory Tests == [assert_dict_foreach_count] Error: got count %d but expected %d", foreachDictCount, 3);
    }
    foreachDictCount = 0;
    dict_free(foreachDict);
    logResult("assert_dict_foreach_count", usedBefore, heap_bytes_used());
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

/*
 * Strings memory leak tests
 */

void assert_substr()
{
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

/*
 * JSON memory leak tests
 */

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

/*
 * Text Layer Reconciler memory leak tests
 */

void assert_text_reconciler_init_deinit()
{
    // Setup
    animation_registry_init();
    layer_registry_init();

    int usedBefore = heap_bytes_used();
    text_layer_reconciler_init();
    text_layer_reconciler_deinit();
    logResult("assert_text_reconciler_init_deinit", usedBefore, heap_bytes_used());

    // Teardown
    animation_registry_deinit();
    layer_registry_deinit();
}

void assert_text_reconciler_add_remove(Window *mainWindow)
{
    // // Setup
    // Layer *parentLayer = window_get_root_layer(mainWindow);
    // animation_registry_init();
    // layer_registry_init();
    // text_layer_reconciler_init();

    // TextLayerPropsMessage textLayerProps = TextLayerPropsMessage_init_default;
    // int usedBefore = heap_bytes_used();
    // text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test", &textLayerProps);
    // text_layer_reconciler(parentLayer, OPERATION_REMOVE_CHILD, NODE_TYPE_TEXT_LAYER, "test", NULL);
    // logResult("assert_text_reconciler_add_remove", usedBefore, heap_bytes_used());

    // // Teardown
    // text_layer_reconciler_deinit();
    // animation_registry_deinit();
    // layer_registry_deinit();
}

void assert_text_reconciler_commit_update(Window *mainWindow)
{
    // // Setup
    // Layer *parentLayer = window_get_root_layer(mainWindow);
    // animation_registry_init();
    // layer_registry_init();
    // text_layer_reconciler_init();

    // TextLayerPropsMessage initialProps = TextLayerPropsMessage_init_default;
    // text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test", &initialProps);

    // TextLayerPropsMessage updateProps = TextLayerPropsMessage_init_default;
    // updateProps.alignment = calloc(strlen("center") + 1, sizeof(char));
    // strcpy(updateProps.alignment, "center");
    // updateProps.text = calloc(strlen("text") + 1, sizeof(char));
    // strcpy(updateProps.text, "text");
    // updateProps.top = 50;
    // int usedBefore = heap_bytes_used();
    // text_layer_reconciler(parentLayer, OPERATION_COMMIT_UPDATE, NODE_TYPE_TEXT_LAYER, "test", &updateProps);

    // logResult("assert_text_reconciler_commit_update", usedBefore, heap_bytes_used());
    // text_layer_reconciler(parentLayer, OPERATION_REMOVE_CHILD, NODE_TYPE_TEXT_LAYER, "test", NULL);

    // // Teardown
    // text_layer_reconciler_deinit();
    // animation_registry_deinit();
    // layer_registry_deinit();
}

void assert_text_reconciler_remove_leftovers(Window *mainWindow)
{
    // // Setup
    // Layer *parentLayer = window_get_root_layer(mainWindow);
    // animation_registry_init();
    // layer_registry_init();

    // int usedBefore = heap_bytes_used();
    // text_layer_reconciler_init();
    // // One
    // TextLayerPropsMessage props1 = TextLayerPropsMessage_init_default;
    // props1.text = calloc(2, sizeof(char));
    // strcpy(props1.text, "a");
    // text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test1", &props1);
    // // Two
    // TextLayerPropsMessage props2 = TextLayerPropsMessage_init_default;
    // text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test2", &props2);
    // // Three
    // TextLayerPropsMessage props3 = TextLayerPropsMessage_init_default;
    // text_layer_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_TEXT_LAYER, "test3", &props3);
    // // Deinit should clean all
    // text_layer_reconciler_deinit();
    // logResult("assert_text_reconciler_remove_leftovers", usedBefore, heap_bytes_used());

    // // Teardown
    // animation_registry_deinit();
    // layer_registry_deinit();
}

/*
 * Animation Reconciler memory leak tests
 */

void assert_animation_reconciler_init_deinit()
{
    // Setup
    animation_registry_init();
    layer_registry_init();

    int usedBefore = heap_bytes_used();
    animation_reconciler_init();
    animation_reconciler_deinit();
    logResult("assert_animation_reconciler_init_deinit", usedBefore, heap_bytes_used());

    // Teardown
    animation_registry_deinit();
    layer_registry_deinit();
}

void assert_animation_reconciler_add_remove(Window *mainWindow)
{
    // // Setup
    // Layer *parentLayer = window_get_root_layer(mainWindow);
    // animation_registry_init();
    // layer_registry_init();
    // animation_reconciler_init();
    // text_layer_reconciler_init();

    // AnimationPropsMessage animationProps = AnimationPropsMessage_init_default;
    // // Start Operations
    // OperationMessage *startOperations = calloc(1, sizeof(OperationMessage));
    // OperationMessage startOperation = OperationMessage_init_default;
    // startOperation.nodeType = NODE_TYPE_TEXT_LAYER;
    // startOperation.nodeId = calloc(strlen("startText") + 1, sizeof(char));
    // strcpy(startOperation.nodeId, "startText");
    // startOperation.operation = OPERATION_COMMIT_UPDATE;
    // // Text Start Props
    // TextLayerPropsMessage *textLayerStartProps = malloc(sizeof(TextLayerPropsMessage));
    // textLayerStartProps->top = 0;
    // textLayerStartProps->topChanged = 1;
    // startOperation.textLayerProps = textLayerStartProps;
    // // End Operations
    // OperationMessage *endOperations = calloc(1, sizeof(OperationMessage));
    // OperationMessage endOperation = OperationMessage_init_default;
    // endOperation.nodeType = NODE_TYPE_TEXT_LAYER;
    // endOperation.nodeId = calloc(strlen("endText") + 1, sizeof(char));
    // strcpy(endOperation.nodeId, "endText");
    // endOperation.operation = OPERATION_COMMIT_UPDATE;
    // // Text end Props
    // TextLayerPropsMessage *textLayerEndProps = malloc(sizeof(TextLayerPropsMessage));
    // textLayerEndProps->top = 50;
    // textLayerEndProps->topChanged = 1;
    // endOperation.textLayerProps = textLayerEndProps;
    // //
    // startOperations[0] = startOperation;
    // animationProps.startOperations = startOperations;
    // animationProps.startOperations_count = 1;
    // endOperations[0] = endOperation;
    // animationProps.endOperations = endOperations;
    // animationProps.endOperations_count = 1;

    // int usedBefore = heap_bytes_used();
    // animation_reconciler(parentLayer, OPERATION_APPEND_CHILD, NODE_TYPE_ANIMATION, "test", &animationProps);
    // animation_reconciler(parentLayer, OPERATION_REMOVE_CHILD, NODE_TYPE_ANIMATION, "test", NULL);
    // logResult("assert_animation_reconciler_add_remove", usedBefore, heap_bytes_used());

    // // Teardown
    // animation_reconciler_deinit();
    // text_layer_reconciler_deinit();
    // animation_registry_deinit();
    // layer_registry_deinit();
}

/*
 * Operations memory leak tests
 */
void assert_operation_copy()
{
    // // Create Operation
    // OperationMessage sourceOperation = OperationMessage_init_default;
    // sourceOperation.nodeType = NODE_TYPE_TEXT_LAYER;
    // sourceOperation.nodeId = calloc(strlen("nodeId") + 1, sizeof(char));
    // strcpy(sourceOperation.nodeId, "nodeId");
    // sourceOperation.operation = OPERATION_COMMIT_UPDATE;
    // // Set Text Props
    // TextLayerPropsMessage *textLayerProps = malloc(sizeof(TextLayerPropsMessage));
    // textLayerProps->top = 0;
    // textLayerProps->topChanged = 1;
    // textLayerProps->text = calloc(strlen("text") + 1, sizeof(char));
    // strcpy(textLayerProps->text, "text");
    // sourceOperation.textLayerProps = textLayerProps;

    // OperationMessage *copy = malloc(sizeof(OperationMessage));

    // int usedBefore = heap_bytes_used();
    // operation_copy(copy, sourceOperation);

    // free(copy->textLayerProps->alignment);
    // free(copy->textLayerProps->text);
    // free(copy->textLayerProps);
    // free(copy->nodeId);
    // free(copy);

    // logResult("assert_operation_copy", usedBefore, heap_bytes_used());
}

void assert_operations_process_empty_batch(Window *mainWindow)
{
    BatchOperationsMessage batchOperations = BatchOperationsMessage_init_default;
    int usedBefore = heap_bytes_used();
    operations_process_batch(mainWindow, &batchOperations);
    logResult("assert_operations_process_batch", usedBefore, heap_bytes_used());
}

void assert_operations_process_batch(Window *mainWindow)
{
    // // Setup
    // animation_registry_init();
    // layer_registry_init();
    // text_layer_reconciler_init();

    // BatchOperationsMessage *batchOperations = malloc(sizeof(BatchOperationsMessage));
    // batchOperations->operations_count = 2;
    // batchOperations->operations = calloc(batchOperations->operations_count, sizeof(OperationMessage));

    // // Append Text
    // batchOperations->operations[0].nodeId = calloc(strlen("text0") + 1, sizeof(char));
    // strcpy(batchOperations->operations[0].nodeId, "text0");
    // batchOperations->operations[0].nodeType = NODE_TYPE_TEXT_LAYER;
    // batchOperations->operations[0].operation = OPERATION_APPEND_CHILD;

    // TextLayerPropsMessage *textLayerProps0 = malloc(sizeof(TextLayerPropsMessage));
    // textLayerProps0->top = 0;
    // textLayerProps0->left = 0;
    // textLayerProps0->width = 0;
    // textLayerProps0->height = 0;
    // textLayerProps0->text = calloc(strlen("text") + 1, sizeof(char));
    // strcpy(textLayerProps0->text, "text");
    // textLayerProps0->alignment = calloc(strlen("center"), sizeof(char));
    // strcpy(textLayerProps0->alignment, "center");
    // batchOperations->operations[0].textLayerProps = textLayerProps0;

    // // Remove Text
    // batchOperations->operations[1].nodeId = calloc(strlen("text0") + 1, sizeof(char));
    // strcpy(batchOperations->operations[1].nodeId, "text0");
    // batchOperations->operations[1].nodeType = NODE_TYPE_TEXT_LAYER;
    // batchOperations->operations[1].operation = OPERATION_REMOVE_CHILD;

    // int usedBefore = heap_bytes_used();
    // operations_process_batch(mainWindow, batchOperations);
    // logResult("assert_operations_process_batch", usedBefore, heap_bytes_used());

    // // Teardown
    // free(batchOperations);
    // text_layer_reconciler_deinit();
    // animation_registry_deinit();
    // layer_registry_deinit();
}