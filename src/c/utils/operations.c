#include "operations.h"
#include "../reconcilers/constants.h"
#include "../reconcilers/animation.h"
#include "../reconcilers/text_layer.h"

OperationMessage *operation_copy(OperationMessage *copy, OperationMessage operation)
{
  copy->nodeId = calloc(strlen(operation.nodeId) + 1, sizeof(char));
  strcpy(copy->nodeId, operation.nodeId);
  copy->nodeType = operation.nodeType;
  copy->operation = operation.operation;

  switch (operation.nodeType)
  {
  case NODE_TYPE_TEXT_LAYER:
  {
    copy->textLayerProps = malloc(sizeof(TextLayerPropsMessage));
    copy->textLayerProps->alignment = NULL;
    copy->textLayerProps->layerProps = NULL;
    copy->textLayerProps->text = NULL;

    text_layer_reconciler_merge_props(copy->textLayerProps, operation.textLayerProps, true);
  }
  break;
  default:
    break;
  }

  operation.operation = OPERATION_CLEAR_PROPS;
  operations_process_unit(NULL, &operation);

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
    TextLayerPropsMessage *props = operationMessage->textLayerProps;

    text_layer_reconciler(windowLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      text_layer_reconciler(NULL, operationMessage);
    }
  }
  break;
  case NODE_TYPE_ANIMATION:
  {
    AnimationPropsMessage *props = operationMessage->animationProps;

    animation_reconciler(windowLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      animation_reconciler(NULL, operationMessage);
    }
  }
  default:
    break;
  }

  free(operationMessage->nodeId);

  // // Reconcilers
  // image_layer_reconciler(windowLayer, operation, nodeType, nodeId, propsDict);
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