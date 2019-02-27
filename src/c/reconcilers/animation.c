#include "animation.h"

#include "../lib/hashmap/hashmap.h"
#include "../utils/json_utils.h"
#include "../utils/layer_props_utils.h"
#include "../utils/layers_registry.h"
#include "../utils/animation_registry.h"
#include "../utils/string.h"
#include "../utils/operations.h"

static PebbleDictionary *animationsRegistry;

static PebbleHashMap *startHashMap;
static PebbleHashMap *endHashMap;

typedef void (*AnimationCallback)(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    int percent);

typedef struct _AnimationInfo
{
  Animation **animations;
  uint8_t length;

} AnimationInfo;

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
    // Save hashmaps
    OperationMessage *startCopy = malloc(sizeof(OperationMessage));
    hash_add(startHashMap, animation, operation_copy(startCopy, props->startOperations[i]));
    OperationMessage *endCopy = malloc(sizeof(OperationMessage));
    hash_add(endHashMap, animation, operation_copy(endCopy, props->endOperations[i]));
    // Add to array
    animations[i] = animation;

    animation_schedule(animation);
  }

  return animations;
}

static void handleAnimationTeardown(Animation *animation)
{
  if (hash_has(startHashMap, animation))
  {
    OperationMessage *startOperation = (OperationMessage *)hash_get(startHashMap, animation);

    // Need to call proper reconciler
    free(startOperation->textLayerProps->alignment);
    free(startOperation->textLayerProps->text);
    free(startOperation->textLayerProps);
    free(startOperation->nodeId);
    free(startOperation);
    hash_remove(startHashMap, animation);
  }
  if (hash_has(endHashMap, animation))
  {
    OperationMessage *endOperation = (OperationMessage *)hash_get(endHashMap, animation);

    // Need to call proper reconciler
    free(endOperation->textLayerProps->alignment);
    free(endOperation->textLayerProps->text);
    free(endOperation->textLayerProps);
    free(endOperation->nodeId);
    free(endOperation);
    hash_remove(endHashMap, animation);
  }

  animation_destroy(animation);
}

static void handleAnimationUpdate(Animation *animation, const AnimationProgress progress)
{
  // Return if, for some reason, we can't find info about the current animation.
  if (
      animation == NULL ||
      !animation_is_scheduled(animation) ||
      !hash_has(startHashMap, animation))
  {
    return;
  }

  // Calculate current percentage
  int percent = ((int)progress * 100) / ANIMATION_NORMALIZED_MAX;

  OperationMessage *startOperation = (OperationMessage *)hash_get(startHashMap, animation);
  OperationMessage *endOperation = (OperationMessage *)hash_get(endHashMap, animation);

  callAnimationCallback = animation_registry_get_callback(startOperation->nodeType);
  callAnimationCallback(startOperation, endOperation, percent);
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

  startHashMap = hash_new();
  endHashMap = hash_new();
}

static void freeRegistryEntry(char *key, void *value)
{
  removeChild(key, false);
}

void animation_reconciler_deinit()
{
  dict_foreach(animationsRegistry, freeRegistryEntry);
  dict_free(animationsRegistry);

  hash_free(startHashMap);
  hash_free(endHashMap);
}

void animation_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    AnimationPropsMessage *props)
{
  if (nodeType != NODE_TYPE_ANIMATION)
  {
    return;
  }

  switch (operation)
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
  default:
    break;
  }
}