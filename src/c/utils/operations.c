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