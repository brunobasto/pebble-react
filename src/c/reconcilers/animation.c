#include "animation.h"

#include "../lib/hashmap/hashmap.h"
#include "../utils/layers_registry.h"
#include "../utils/animation_registry.h"
#include "../utils/string.h"
#include "../utils/operations.h"

static PebbleDictionary *animationsRegistry;

static PebbleHashMap *operationsHashMap;

typedef void (*AnimationCallback)(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    OperationMessage *resultOperation,
    int percent);

typedef struct _AnimationInfo
{
  Animation **animations;
  uint8_t length;

} AnimationInfo;

typedef struct _AnimationOperationsInfo
{
  OperationMessage *startOperation;
  OperationMessage *endOperation;

} AnimationOperationsInfo;

static AnimationCallback callAnimationCallback = NULL;

static void handleAnimationUpdate(Animation *animation, const AnimationProgress progress);
static void handleAnimationTeardown(Animation *animation);

static AnimationImplementation impl = {.update = handleAnimationUpdate, .teardown = handleAnimationTeardown};

static void setAnimationProperties(Animation *animation, AnimationPropsMessage *props)
{
  if (props == NULL)
  {
    return;
  }
  // Duration
  animation_set_duration(animation, props->duration);

  // Curve
  animation_set_curve(animation, AnimationCurveEaseInOut);

  // Implementation
  animation_set_implementation(animation, &impl);
}

static Animation **createAnimations(AnimationPropsMessage *props)
{
  // Create the arrays to hold the values
  Animation **animations = calloc(props->startOperations_count, sizeof(Animation *));

  for (uint16_t i = 0; i < props->startOperations_count; i++)
  {
    Animation *animation = animation_create();
    // Setup
    setAnimationProperties(animation, props);
    // Save to hashmap
    OperationMessage *startCopy = malloc(sizeof(OperationMessage));
    OperationMessage *endCopy = malloc(sizeof(OperationMessage));

    AnimationOperationsInfo *info = malloc(sizeof(AnimationOperationsInfo));
    info->startOperation = operation_copy(startCopy, props->startOperations[i]);
    info->endOperation = operation_copy(endCopy, props->endOperations[i]);

    hash_add(operationsHashMap, animation, info);
    // Add to array
    animations[i] = animation;
    // Schedule
    animation_schedule(animation);
  }

  return animations;
}

static void handleAnimationTeardown(Animation *animation)
{
  AnimationOperationsInfo *info = (AnimationOperationsInfo *)hash_get(operationsHashMap, animation);

  if (info)
  {
    OperationMessage *startOperation = info->startOperation;
    startOperation->operation = OPERATION_CLEAR_PROPS;
    operations_process_unit(NULL, startOperation);
    free(startOperation);

    OperationMessage *endOperation = info->endOperation;
    endOperation->operation = OPERATION_CLEAR_PROPS;
    operations_process_unit(NULL, endOperation);
    free(endOperation);

    hash_remove(operationsHashMap, animation);
    free(info);
  }

  animation_destroy(animation);
}

static void handleAnimationUpdate(Animation *animation, const AnimationProgress progress)
{
  AnimationOperationsInfo *info = (AnimationOperationsInfo *)hash_get(operationsHashMap, animation);
  // Return if, for some reason, we can't find info about the current animation.
  if (
      animation == NULL ||
      !info ||
      !animation_is_scheduled(animation))
  {
    return;
  }

  // Calculate current percentage
  int percent = ((int)progress * 100) / ANIMATION_NORMALIZED_MAX;

  OperationMessage *startOperation = info->startOperation;
  OperationMessage *endOperation = info->endOperation;
  OperationMessage *resultOperation = malloc(sizeof(OperationMessage));
  resultOperation->operation = OPERATION_COMMIT_UPDATE;
  resultOperation->nodeType = startOperation->nodeType;
  resultOperation->nodeId = calloc(strlen(startOperation->nodeId) + 1, sizeof(char));
  strcpy(resultOperation->nodeId, startOperation->nodeId);
  resultOperation->parentNodeId = calloc(strlen(startOperation->parentNodeId) + 1, sizeof(char));
  strcpy(resultOperation->parentNodeId, startOperation->parentNodeId);

  callAnimationCallback = animation_registry_get_callback(startOperation->nodeType);
  callAnimationCallback(startOperation, endOperation, resultOperation, percent);

  operations_process_unit(NULL, resultOperation);

  free(resultOperation);
}

static void appendChild(
    const char *nodeId,
    AnimationPropsMessage *props)
{
  // Animations
  uint16_t animationsLength = props->startOperations_count;

  Animation **animations = createAnimations(props);

  // // Sequence?
  // if (props->sequence)
  // {
  //   composed = animation_sequence_create_from_array(animations, animationsLength);
  // }
  // else
  // {
  //   composed = animation_spawn_create_from_array(animations, animationsLength);
  // }

  // Loop?
  // if (props->loop)
  // {
  //   animation_set_play_count(composed, ANIMATION_DURATION_INFINITE);
  // }

  AnimationInfo *animationInfo = malloc(sizeof(AnimationInfo));

  animationInfo->animations = animations;
  animationInfo->length = animationsLength;

  // Saving on registry for later use (commitUpdate and removeChild)
  dict_add(animationsRegistry, nodeId, animationInfo);

  free(props->endOperations);
  free(props->startOperations);
}

static void removeChild(const char *nodeId, bool removeFromRegistry)
{
  AnimationInfo *animationInfo = (AnimationInfo *)dict_get(animationsRegistry, nodeId);

  Animation **animations = animationInfo->animations;
  uint16_t animationsLength = animationInfo->length;

  for (uint16_t i = 0; i < animationsLength; i++)
  {
    if (animation_is_scheduled(animations[i]))
      animation_unschedule(animations[i]);
  }

  free(animations);
  free(animationInfo);

  if (removeFromRegistry)
  {
    dict_remove(animationsRegistry, nodeId);
  }
}

static void commitUpdate(
    const char *nodeId,
    AnimationPropsMessage *props)
{
  removeChild(nodeId, true);
  appendChild(nodeId, props);
}

// Public

void animation_reconciler_init()
{
  animationsRegistry = dict_new();
  operationsHashMap = hash_new();
}

static void freeRegistryEntry(char *key, void *value)
{
  removeChild(key, false);
}

void animation_reconciler_deinit()
{
  dict_foreach(animationsRegistry, freeRegistryEntry);
  dict_free(animationsRegistry);
  hash_free(operationsHashMap);
}

static void clearProps(AnimationPropsMessage *props) {
  free(props);
}

void animation_reconciler(
    Layer *parentLayer,
    OperationMessage *operationMessage)
{
  uint8_t nodeType = operationMessage->nodeType;
  AnimationPropsMessage *props = operationMessage->animationProps;
  char *nodeId = operationMessage->nodeId;

  if (nodeType != NODE_TYPE_ANIMATION)
  {
    return;
  }

  switch (operationMessage->operation)
  {
  case OPERATION_APPEND_CHILD:
    appendChild(nodeId, props);
    break;
  case OPERATION_COMMIT_UPDATE:
    commitUpdate(nodeId, props);
    break;
  case OPERATION_REMOVE_CHILD:
    removeChild(nodeId, true);
    break;
  case OPERATION_CLEAR_PROPS:
    clearProps(props);
    break;
  default:
    break;
  }
}