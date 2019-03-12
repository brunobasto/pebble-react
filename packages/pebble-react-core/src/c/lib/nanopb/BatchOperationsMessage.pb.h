/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.2 at Mon Mar 11 18:12:25 2019. */

#ifndef PB_BATCHOPERATIONSMESSAGE_PB_H_INCLUDED
#define PB_BATCHOPERATIONSMESSAGE_PB_H_INCLUDED
#include "pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _BatchOperationsMessage {
    pb_size_t operations_count;
    struct _OperationMessage *operations;
/* @@protoc_insertion_point(struct:BatchOperationsMessage) */
} BatchOperationsMessage;

typedef struct _AnimationPropsMessage {
    bool loop;
    bool sequence;
    pb_size_t endOperations_count;
    struct _OperationMessage *endOperations;
    pb_size_t startOperations_count;
    struct _OperationMessage *startOperations;
    uint16_t delay;
    uint16_t duration;
    bool durationChanged;
    bool delayChanged;
    bool loopChanged;
    bool sequenceChanged;
/* @@protoc_insertion_point(struct:AnimationPropsMessage) */
} AnimationPropsMessage;

typedef struct _ArcLayerPropsMessage {
    int16_t endAngle;
    uint8_t radius;
    int16_t startAngle;
    struct _LayerPropsMessage *layerProps;
    uint32_t strokeColor;
    uint8_t strokeWidth;
    uint8_t thickness;
    uint32_t color;
    bool endAngleChanged;
    bool startAngleChanged;
    bool radiusChanged;
    bool layerPropsChanged;
    bool strokeColorChanged;
    bool strokeWidthChanged;
    bool thicknessChanged;
    bool colorChanged;
/* @@protoc_insertion_point(struct:ArcLayerPropsMessage) */
} ArcLayerPropsMessage;

typedef struct _CircleLayerPropsMessage {
    uint8_t radius;
    struct _LayerPropsMessage *layerProps;
    bool radiusChanged;
    bool layerPropsChanged;
/* @@protoc_insertion_point(struct:CircleLayerPropsMessage) */
} CircleLayerPropsMessage;

typedef struct _LayerPropsMessage {
    uint8_t height;
    int16_t left;
    int16_t top;
    uint8_t width;
    bool heightChanged;
    bool leftChanged;
    bool topChanged;
    bool widthChanged;
/* @@protoc_insertion_point(struct:LayerPropsMessage) */
} LayerPropsMessage;

typedef struct _OperationMessage {
    uint8_t operation;
    uint8_t nodeType;
    char *nodeId;
    char *parentNodeId;
    struct _TextLayerPropsMessage *textLayerProps;
    struct _AnimationPropsMessage *animationProps;
    struct _LayerPropsMessage *layerProps;
    struct _CircleLayerPropsMessage *circleLayerProps;
    struct _ArcLayerPropsMessage *arcLayerProps;
    struct _PathLayerPropsMessage *pathLayerProps;
/* @@protoc_insertion_point(struct:OperationMessage) */
} OperationMessage;

typedef struct _PathLayerPropsMessage {
    uint32_t strokeColor;
    uint32_t fillColor;
    pb_size_t points_count;
    struct _PointMessage *points;
    bool strokeColorChanged;
    bool fillColorChanged;
    bool pointsChanged;
/* @@protoc_insertion_point(struct:PathLayerPropsMessage) */
} PathLayerPropsMessage;

typedef struct _PointMessage {
    int16_t x;
    int16_t y;
    bool xChanged;
    bool yChanged;
/* @@protoc_insertion_point(struct:PointMessage) */
} PointMessage;

typedef struct _TextLayerPropsMessage {
    char *alignment;
    uint32_t color;
    char *text;
    struct _LayerPropsMessage *layerProps;
    bool alignmentChanged;
    bool colorChanged;
    bool textChanged;
    bool layerPropsChanged;
/* @@protoc_insertion_point(struct:TextLayerPropsMessage) */
} TextLayerPropsMessage;

/* Default values for struct fields */

/* Initializer values for message structs */
#define LayerPropsMessage_init_default           {0, 0, 0, 0, 0, 0, 0, 0}
#define TextLayerPropsMessage_init_default       {NULL, 0, NULL, NULL, 0, 0, 0, 0}
#define ArcLayerPropsMessage_init_default        {0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define PointMessage_init_default                {0, 0, 0, 0}
#define PathLayerPropsMessage_init_default       {0, 0, 0, NULL, 0, 0, 0}
#define CircleLayerPropsMessage_init_default     {0, NULL, 0, 0}
#define AnimationPropsMessage_init_default       {0, 0, 0, NULL, 0, NULL, 0, 0, 0, 0, 0, 0}
#define OperationMessage_init_default            {0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
#define BatchOperationsMessage_init_default      {0, NULL}
#define LayerPropsMessage_init_zero              {0, 0, 0, 0, 0, 0, 0, 0}
#define TextLayerPropsMessage_init_zero          {NULL, 0, NULL, NULL, 0, 0, 0, 0}
#define ArcLayerPropsMessage_init_zero           {0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define PointMessage_init_zero                   {0, 0, 0, 0}
#define PathLayerPropsMessage_init_zero          {0, 0, 0, NULL, 0, 0, 0}
#define CircleLayerPropsMessage_init_zero        {0, NULL, 0, 0}
#define AnimationPropsMessage_init_zero          {0, 0, 0, NULL, 0, NULL, 0, 0, 0, 0, 0, 0}
#define OperationMessage_init_zero               {0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
#define BatchOperationsMessage_init_zero         {0, NULL}

/* Field tags (for use in manual encoding/decoding) */
#define BatchOperationsMessage_operations_tag    1
#define AnimationPropsMessage_loop_tag           1
#define AnimationPropsMessage_sequence_tag       2
#define AnimationPropsMessage_endOperations_tag  3
#define AnimationPropsMessage_startOperations_tag 4
#define AnimationPropsMessage_delay_tag          5
#define AnimationPropsMessage_duration_tag       6
#define AnimationPropsMessage_durationChanged_tag 7
#define AnimationPropsMessage_delayChanged_tag   8
#define AnimationPropsMessage_loopChanged_tag    9
#define AnimationPropsMessage_sequenceChanged_tag 10
#define ArcLayerPropsMessage_endAngle_tag        1
#define ArcLayerPropsMessage_radius_tag          2
#define ArcLayerPropsMessage_startAngle_tag      3
#define ArcLayerPropsMessage_layerProps_tag      4
#define ArcLayerPropsMessage_strokeColor_tag     5
#define ArcLayerPropsMessage_strokeWidth_tag     6
#define ArcLayerPropsMessage_thickness_tag       7
#define ArcLayerPropsMessage_color_tag           8
#define ArcLayerPropsMessage_endAngleChanged_tag 9
#define ArcLayerPropsMessage_startAngleChanged_tag 10
#define ArcLayerPropsMessage_radiusChanged_tag   11
#define ArcLayerPropsMessage_layerPropsChanged_tag 12
#define ArcLayerPropsMessage_strokeColorChanged_tag 13
#define ArcLayerPropsMessage_strokeWidthChanged_tag 14
#define ArcLayerPropsMessage_thicknessChanged_tag 15
#define ArcLayerPropsMessage_colorChanged_tag    16
#define CircleLayerPropsMessage_radius_tag       1
#define CircleLayerPropsMessage_layerProps_tag   2
#define CircleLayerPropsMessage_radiusChanged_tag 3
#define CircleLayerPropsMessage_layerPropsChanged_tag 4
#define LayerPropsMessage_height_tag             1
#define LayerPropsMessage_left_tag               2
#define LayerPropsMessage_top_tag                3
#define LayerPropsMessage_width_tag              4
#define LayerPropsMessage_heightChanged_tag      5
#define LayerPropsMessage_leftChanged_tag        6
#define LayerPropsMessage_topChanged_tag         7
#define LayerPropsMessage_widthChanged_tag       8
#define OperationMessage_operation_tag           1
#define OperationMessage_nodeType_tag            2
#define OperationMessage_nodeId_tag              3
#define OperationMessage_parentNodeId_tag        4
#define OperationMessage_textLayerProps_tag      5
#define OperationMessage_animationProps_tag      6
#define OperationMessage_layerProps_tag          7
#define OperationMessage_circleLayerProps_tag    8
#define OperationMessage_arcLayerProps_tag       9
#define OperationMessage_pathLayerProps_tag      10
#define PathLayerPropsMessage_strokeColor_tag    1
#define PathLayerPropsMessage_fillColor_tag      2
#define PathLayerPropsMessage_points_tag         3
#define PathLayerPropsMessage_strokeColorChanged_tag 4
#define PathLayerPropsMessage_fillColorChanged_tag 5
#define PathLayerPropsMessage_pointsChanged_tag  6
#define PointMessage_x_tag                       1
#define PointMessage_y_tag                       2
#define PointMessage_xChanged_tag                3
#define PointMessage_yChanged_tag                4
#define TextLayerPropsMessage_alignment_tag      1
#define TextLayerPropsMessage_color_tag          2
#define TextLayerPropsMessage_text_tag           3
#define TextLayerPropsMessage_layerProps_tag     4
#define TextLayerPropsMessage_alignmentChanged_tag 5
#define TextLayerPropsMessage_colorChanged_tag   6
#define TextLayerPropsMessage_textChanged_tag    7
#define TextLayerPropsMessage_layerPropsChanged_tag 8

/* Struct field encoding specification for nanopb */
extern const pb_field_t LayerPropsMessage_fields[9];
extern const pb_field_t TextLayerPropsMessage_fields[9];
extern const pb_field_t ArcLayerPropsMessage_fields[17];
extern const pb_field_t PointMessage_fields[5];
extern const pb_field_t PathLayerPropsMessage_fields[7];
extern const pb_field_t CircleLayerPropsMessage_fields[5];
extern const pb_field_t AnimationPropsMessage_fields[11];
extern const pb_field_t OperationMessage_fields[11];
extern const pb_field_t BatchOperationsMessage_fields[2];

/* Maximum encoded size of messages (where known) */
#define LayerPropsMessage_size                   42
/* TextLayerPropsMessage_size depends on runtime parameters */
/* ArcLayerPropsMessage_size depends on runtime parameters */
#define PointMessage_size                        26
/* PathLayerPropsMessage_size depends on runtime parameters */
/* CircleLayerPropsMessage_size depends on runtime parameters */
/* AnimationPropsMessage_size depends on runtime parameters */
/* OperationMessage_size depends on runtime parameters */
/* BatchOperationsMessage_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define BATCHOPERATIONSMESSAGE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
