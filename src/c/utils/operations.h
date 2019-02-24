#include <pebble.h>

#include "../lib/nanopb/BatchOperationsMessage.pb.h"

OperationMessage *operation_copy(OperationMessage *copy, OperationMessage operation);
void operations_process_batch(Window *mainWindow, BatchOperationsMessage *batchOperations);
void operations_process_unit(Window *mainWindow, OperationMessage *operationMessage);