#include "operations.h"
#include "../reconcilers/constants.h"
#include "../reconcilers/text_layer.h"

OperationMessage *operation_copy(OperationMessage operation)
{
    OperationMessage *copy = malloc(sizeof(OperationMessage *));
    strcpy(copy->nodeId, operation.nodeId);
    copy->nodeType = operation.nodeType;
    copy->operation = operation.operation;

    switch (operation.nodeType)
    {
    case NODE_TYPE_TEXT_LAYER:
    {
        TextLayerPropsMessage *props = malloc(sizeof(TextLayerPropsMessage));

        text_layer_reconciler_merge_props(props, copy->textLayerProps, true);
        copy->textLayerProps = props;
    }
    break;
    default:
        break;
    }

    return copy;
}

void operations_process_unit(Window *mainWindow, OperationMessage *operationMessage)
{
  const uint8_t operation = operationMessage->operation;
  const uint8_t nodeType = operationMessage->nodeType;
  const char *nodeId = operationMessage->nodeId;

  Layer *windowLayer = window_get_root_layer(mainWindow);

  switch (nodeType)
  {
  case NODE_TYPE_TEXT_LAYER:
  {
    TextLayerPropsMessage *textLayerProps = operationMessage->textLayerProps;

    text_layer_reconciler(windowLayer, operation, nodeType, nodeId, textLayerProps);
    free(textLayerProps);
  }
  break;

  default:
    break;
  }

  free(operationMessage->nodeId);

  // // Reconcilers
  // text_layer_reconciler(windowLayer, operation, nodeType, nodeId, propsDict);
  // image_layer_reconciler(windowLayer, operation, nodeType, nodeId, propsDict);
  // animation_reconciler(windowLayer, operation, nodeType, nodeId, propsDict);
}

void operations_process_batch(Window *mainWindow, BatchOperationsMessage *batchOperations)
{
  APP_LOG(APP_LOG_LEVEL_INFO, "got %d operations", batchOperations->operations_count);

  for (uint16_t i = 0; i < batchOperations->operations_count; i++)
  {
    OperationMessage operationMessage = batchOperations->operations[i];

    operations_process_unit(mainWindow, &operationMessage);
  }

  free(batchOperations->operations);
}