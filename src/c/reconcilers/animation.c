#include "animation.h"

#include "../lib/hashmap/hashmap.h"
#include "../utils/json_utils.h"
#include "../utils/layer_props_utils.h"
#include "../utils/layers_registry.h"
#include "../utils/animation_registry.h"
#include "../utils/string.h"
#include "../utils/operations.h"

static PebbleDictionary *animationsRegistry;
static PebbleDictionary *animationsLengthRegistry;

static PebbleHashMap *startHashMap;
static PebbleHashMap *endHashMap;

typedef void (*AnimationCallback)(
    OperationMessage *startOperation,
    OperationMessage *endOperation,
    int percent);

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

static Animation **registerAnimations(
    const char *nodeId,
    AnimationPropsMessage *props)
{
  // Create the arrays to hold the values
  Animation **animations = malloc(props->startOperations_count * sizeof(Animation *));

  for (uint16_t i = 0; i < props->startOperations_count; i++)
  {
    Animation *animation = animation_create();
    // Setup
    setAnimationProperties(animation, props);
    // Save hashmaps
    // Need to copy or the memory on stack will get freed
    // need to rememember to deallocate this later
    hash_add(startHashMap, animation, operation_copy(props->startOperations[i]));
    hash_add(endHashMap, animation, operation_copy(props->endOperations[i]));
    // Add to array
    animations[i] = animation;
  }

  return animations;
}

static void handleAnimationTeardown(Animation *animation)
{
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
    const uint16_t nodeType,
    const char *nodeId,
    AnimationPropsMessage *props)
{
  // Animations
  uint16_t *animationsLength = malloc(sizeof(uint16_t));
  *animationsLength = props->startOperations_count;

  Animation **animations = registerAnimations(nodeId, props);

  dict_add(animationsLengthRegistry, nodeId, animationsLength);

  Animation *composed;

  // Sequence?
  bool isSequence = false;

  if (isSequence)
  {
    composed = animation_sequence_create_from_array(animations, *animationsLength);
  }
  else
  {
    composed = animation_spawn_create_from_array(animations, *animationsLength);
  }

  // Loop
  // if (dict_has(props, "loop") && atoi((char *)dict_get(props, "loop")))
  // {
  //   animation_set_play_count(composed, ANIMATION_DURATION_INFINITE);
  // }

  dict_add(animationsRegistry, nodeId, animations);

  // Schedule animation
  animation_schedule(composed);
}

static void commitUpdate(
    const uint16_t nodeType,
    const char *nodeId,
    AnimationPropsMessage *props)
{
  // Animation **animations = (Animation **)dict_get(animationsRegistry, nodeId);
  // uint16_t *animationsLength = (uint16_t *)dict_get(animationsLengthRegistry, nodeId);

  // for (uint16_t i = 0; i < *animationsLength; i++)
  // {
  //   animation_unschedule(animations[i]);
  //   setAnimationProperties(animations[i], props);
  //   animation_schedule(animations[i]);
  // }
}

static void removeChild(
    const uint16_t nodeType,
    const char *nodeId)
{
  Animation **animations = (Animation **)dict_get(animationsRegistry, nodeId);
  uint16_t *animationsLength = (uint16_t *)dict_get(animationsLengthRegistry, nodeId);

  for (uint16_t i = 0; i < *animationsLength; i++)
  {
    if (animation_is_scheduled(animations[i]))
    {
      handleAnimationTeardown(animations[i]);
      animation_unschedule(animations[i]);
      animation_destroy(animations[i]);
    }
  }

  dict_remove(animationsRegistry, nodeId);
  dict_remove(animationsLengthRegistry, nodeId);

  free(animations);
  free(animationsLength);
}

// Public

void animation_reconciler_init()
{
  animationsRegistry = dict_new();
  animationsLengthRegistry = dict_new();

  startHashMap = hash_new();
  endHashMap = hash_new();
}

void animation_reconciler_deinit()
{
  animation_unschedule_all();

  dict_free(animationsRegistry);
  dict_free(animationsLengthRegistry);

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
    appendChild(nodeType, nodeId, props);
    break;
  case OPERATION_COMMIT_UPDATE:
    commitUpdate(nodeType, nodeId, props);
    break;
  case OPERATION_REMOVE_CHILD:
    removeChild(nodeType, nodeId);
    break;
  default:
    break;
  }
}