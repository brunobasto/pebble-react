#include "operations.h"

#include "layers_registry.h"
#include "../reconcilers/constants.h"

#include "../reconcilers/animation.h"
#include "../reconcilers/arc_layer.h"
#include "../reconcilers/circle_layer.h"
#include "../reconcilers/layer.h"
#include "../reconcilers/path_layer.h"
#include "../reconcilers/text_layer.h"

OperationMessage *operation_copy(OperationMessage *copy, OperationMessage operation)
{
  copy->nodeId = calloc(strlen(operation.nodeId) + 1, sizeof(char));
  strcpy(copy->nodeId, operation.nodeId);
  copy->parentNodeId = calloc(strlen(operation.parentNodeId) + 1, sizeof(char));
  strcpy(copy->parentNodeId, operation.parentNodeId);
  copy->nodeType = operation.nodeType;
  copy->operation = operation.operation;

  switch (operation.nodeType)
  {
  case NODE_TYPE_LAYER:
  {
    copy->layerProps = malloc(sizeof(LayerPropsMessage));

    layer_reconciler_merge_props(copy->layerProps, operation.layerProps, true);
  }
  break;
  case NODE_TYPE_TEXT_LAYER:
  {
    copy->textLayerProps = malloc(sizeof(TextLayerPropsMessage));
    copy->textLayerProps->alignment = NULL;
    copy->textLayerProps->layerProps = NULL;
    copy->textLayerProps->text = NULL;

    text_layer_reconciler_merge_props(copy->textLayerProps, operation.textLayerProps, true);
  }
  break;
  case NODE_TYPE_PATH_LAYER:
  {
    copy->pathLayerProps = malloc(sizeof(PathLayerPropsMessage));
    copy->pathLayerProps->points = NULL;

    path_layer_reconciler_merge_props(copy->pathLayerProps, operation.pathLayerProps, true);
  }
  break;
  case NODE_TYPE_CIRCLE_LAYER:
  {
    copy->circleLayerProps = malloc(sizeof(CircleLayerPropsMessage));
    copy->circleLayerProps->layerProps = NULL;

    circle_layer_reconciler_merge_props(copy->circleLayerProps, operation.circleLayerProps, true);
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
  const char *parentNodeId = operationMessage->parentNodeId;

  Layer *parentLayer = window_get_root_layer(mainWindow);

  if (
    strcmp(parentNodeId, "ROOT") != 0 &&
    layer_registry_has(parentNodeId)
  )
  {
    parentLayer = layer_registry_get(parentNodeId);
  }

  switch (nodeType)
  {
  case NODE_TYPE_LAYER:
  {
    layer_reconciler(parentLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS && operation != OPERATION_REMOVE_CHILD)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      layer_reconciler(NULL, operationMessage);
    }
  }
  break;
  case NODE_TYPE_TEXT_LAYER:
  {
    text_layer_reconciler(parentLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS && operation != OPERATION_REMOVE_CHILD)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      text_layer_reconciler(NULL, operationMessage);
    }
  }
  break;
  case NODE_TYPE_ARC_LAYER:
  {
    arc_layer_reconciler(parentLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS && operation != OPERATION_REMOVE_CHILD)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      arc_layer_reconciler(NULL, operationMessage);
    }
  }
  break;
  case NODE_TYPE_CIRCLE_LAYER:
  {
    circle_layer_reconciler(parentLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS && operation != OPERATION_REMOVE_CHILD)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      circle_layer_reconciler(NULL, operationMessage);
    }
  }
  break;
  case NODE_TYPE_ANIMATION:
  {
    animation_reconciler(parentLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS && operation != OPERATION_REMOVE_CHILD)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      animation_reconciler(NULL, operationMessage);
    }
  }
  break;
  case NODE_TYPE_PATH_LAYER:
  {
    path_layer_reconciler(parentLayer, operationMessage);

    // Makes sure we clear after ourselves
    if (operation != OPERATION_CLEAR_PROPS && operation != OPERATION_REMOVE_CHILD)
    {
      operationMessage->operation = OPERATION_CLEAR_PROPS;
      path_layer_reconciler(NULL, operationMessage);
    }
  }
  break;
  default:
    break;
  }

  free(operationMessage->nodeId);
  free(operationMessage->parentNodeId);
}

void operations_process_batch(Window *mainWindow, BatchOperationsMessage *batchOperations)
{
  for (uint16_t i = 0; i < batchOperations->operations_count; i++)
  {
    OperationMessage operationMessage = batchOperations->operations[i];

    operations_process_unit(mainWindow, &operationMessage);
  }

  free(batchOperations->operations);
}