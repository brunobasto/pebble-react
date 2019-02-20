#include "animation.h"

#include "../lib/hashmap/hashmap.h"
#include "../utils/json_utils.h"
#include "../utils/layer_props_utils.h"
#include "../utils/layers_registry.h"
#include "../utils/string.h"

typedef struct AnimationInfo_t
{
  uint16_t *childrenLength;
  char **children;
  PebbleDictionary *props;
} AnimationInfo;

static PebbleDictionary *animationsRegistry;
static PebbleDictionary *animationsLengthRegistry;

static PebbleHashMap *animationsInfoHashMap;

typedef void (*ReconcilerCall)(char *nodeId, PebbleDictionary *props);

static ReconcilerCall callReconciler = NULL;

static void handleAnimationUpdate(Animation *animation, const AnimationProgress progress);
static void handleAnimationTeardown(Animation *animation);

static AnimationImplementation impl = {.update = handleAnimationUpdate, .teardown = handleAnimationTeardown};

static void setAnimationProperties(Animation *animation, PebbleDictionary *props)
{
  if (props == NULL)
  {
    return;
  }
  // Duration
  uint32_t duration = atoi((char *)dict_get(props, "duration"));
  animation_set_duration(animation, duration);

  // Curve
  animation_set_curve(animation, AnimationCurveEaseInOut);

  // Implementation
  animation_set_implementation(animation, &impl);
}

static char **getAnimationChildren(PebbleDictionary *props, uint16_t *childrenLength)
{
  // Create the array to hold the values
  char **children = malloc(*childrenLength * sizeof(char *));

  // Parse JSON
  json_utils_parse_array_to_array((char *)dict_get(props, "children"), children);

  return children;
}

static Animation **registerAnimations(
    const char *nodeId,
    PebbleDictionary *props,
    uint16_t animationsLength)
{
  // Create the arrays to hold the values
  char **animationPropsJSONArray = malloc(animationsLength * sizeof(PebbleDictionary *));
  Animation **animations = malloc(animationsLength * sizeof(Animation *));

  // Parse JSON
  json_utils_parse_array_to_array((char *)dict_get(props, "animationProps"), animationPropsJSONArray);

  uint16_t *childrenLength = malloc(sizeof(uint16_t));
  *childrenLength = atoi((char *)dict_get(props, "childrenLength"));

  for (uint16_t i = 0; i < animationsLength; i++)
  {
    Animation *animation = animation_create();
    // Setup
    setAnimationProperties(animation, props);
    // Create animation props dict
    PebbleDictionary *animationProps = dict_new();
    json_utils_parse_object_to_dict(animationPropsJSONArray[i], animationProps);
    free(animationPropsJSONArray[i]);
    // Props
    AnimationInfo *info = malloc(sizeof(AnimationInfo));
    info->childrenLength = childrenLength;
    info->children = getAnimationChildren(props, childrenLength);
    info->props = animationProps;
    // Save hashmap
    hash_add(animationsInfoHashMap, animation, info);
    // Add to array
    animations[i] = animation;
  }

  free(animationPropsJSONArray);

  return animations;
}

static void handleAnimationTeardown(Animation *animation)
{
  // if (hash_has(animationsChildrenLengthHashMap, animation)) {
  //   uint16_t *animationChildrenLength = (uint16_t *)hash_get(animationsChildrenLengthHashMap, animation);

  //   if (animationChildrenLength != NULL) {
  //     free(animationChildrenLength);
  //   }

  //   char **animationChildren = (char **)hash_get(animationsChildrenHashMap, animation);

  //   if (animationChildren != NULL) {
  //     free(animationChildren);
  //   }

  //   hash_add(animationsChildrenHashMap, animation, NULL);
  //   hash_add(animationsChildrenLengthHashMap, animation, NULL);
  // }
}

static void handleAnimationUpdate(Animation *animation, const AnimationProgress progress)
{
  if (animation == NULL || !animation_is_scheduled(animation) || !hash_has(animationsInfoHashMap, animation))
  {
    return;
  }

  AnimationInfo *animationInfo = (AnimationInfo *)hash_get(animationsInfoHashMap, animation);

  char **animationChildren = animationInfo->children;
  uint16_t *animationChildrenLength = animationInfo->childrenLength;
  PebbleDictionary *animationProps = animationInfo->props;

  char *propName = (char *)dict_get(animationProps, "name");

  // Start
  char *startString = (char *)dict_get(animationProps, "start");
  int start = atoi(startString);

  // End
  char *endString = (char *)dict_get(animationProps, "end");
  int end = atoi(endString);

  // Current value
  int percent = ((int)progress * 100) / ANIMATION_NORMALIZED_MAX;
  int current = (int)(start + (end - start) * percent / 100);
  // APP_LOG(APP_LOG_LEVEL_INFO, "seeting real top to %d", current);

  // Create component props from animation props
  PebbleDictionary *props = dict_new();
  char *v = calloc(5 + 1, sizeof(char));
  snprintf(v, 5 + 1, "%d", current);
  dict_add(props, propName, v);

  for (uint16_t i = 0; i < *animationChildrenLength; i++)
  {
    Layer *layer = layer_registry_get(animationChildren[i]);
    callReconciler = layer_registry_get_reconciler(layer);
    char *nodeId = layer_registry_get_node_id(layer);
    callReconciler(nodeId, props);
  }
  free(v);
  dict_remove(props, propName);
  dict_free(props);
}

static void appendChild(
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *props)
{
  // Animations
  uint16_t *animationsLength = malloc(sizeof(uint16_t));
  *animationsLength = atoi((char *)dict_get(props, "animationsLength"));

  Animation **animations = registerAnimations(nodeId, props, *animationsLength);

  dict_add(animationsLengthRegistry, nodeId, animationsLength);

  Animation *composed;

  // Sequence?
  bool isSequence = atoi((char *)dict_get(props, "isSequence"));

  if (isSequence)
  {
    composed = animation_sequence_create_from_array(animations, *animationsLength);
  }
  else
  {
    composed = animation_spawn_create_from_array(animations, *animationsLength);
  }

  // Loop
  if (dict_has(props, "loop") && atoi((char *)dict_get(props, "loop")))
  {
    animation_set_play_count(composed, ANIMATION_DURATION_INFINITE);
  }

  dict_add(animationsRegistry, nodeId, animations);

  // Schedule animation
  animation_schedule(composed);
}

static void commitUpdate(
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *props)
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

  animationsInfoHashMap = hash_new();
}

void animation_reconciler_deinit()
{
  animation_unschedule_all();

  dict_free(animationsRegistry);
  dict_free(animationsLengthRegistry);

  hash_free(animationsInfoHashMap);
}

void animation_reconciler(
    Layer *parentLayer,
    const uint16_t operation,
    const uint16_t nodeType,
    const char *nodeId,
    PebbleDictionary *props)
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