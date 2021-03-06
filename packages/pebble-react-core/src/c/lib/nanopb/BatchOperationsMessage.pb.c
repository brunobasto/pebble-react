/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.2 at Wed Mar 13 12:17:51 2019. */

#include "BatchOperationsMessage.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t LayerPropsMessage_fields[9] = {
    PB_FIELD(  1, UINT32  , SINGULAR, STATIC  , FIRST, LayerPropsMessage, height, height, 0),
    PB_FIELD(  2, INT32   , SINGULAR, STATIC  , OTHER, LayerPropsMessage, left, height, 0),
    PB_FIELD(  3, INT32   , SINGULAR, STATIC  , OTHER, LayerPropsMessage, top, left, 0),
    PB_FIELD(  4, UINT32  , SINGULAR, STATIC  , OTHER, LayerPropsMessage, width, top, 0),
    PB_FIELD(  5, BOOL    , SINGULAR, STATIC  , OTHER, LayerPropsMessage, heightChanged, width, 0),
    PB_FIELD(  6, BOOL    , SINGULAR, STATIC  , OTHER, LayerPropsMessage, leftChanged, heightChanged, 0),
    PB_FIELD(  7, BOOL    , SINGULAR, STATIC  , OTHER, LayerPropsMessage, topChanged, leftChanged, 0),
    PB_FIELD(  8, BOOL    , SINGULAR, STATIC  , OTHER, LayerPropsMessage, widthChanged, topChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t TextLayerPropsMessage_fields[9] = {
    PB_FIELD(  1, STRING  , SINGULAR, POINTER , FIRST, TextLayerPropsMessage, alignment, alignment, 0),
    PB_FIELD(  2, UINT32  , SINGULAR, STATIC  , OTHER, TextLayerPropsMessage, color, alignment, 0),
    PB_FIELD(  3, STRING  , SINGULAR, POINTER , OTHER, TextLayerPropsMessage, text, color, 0),
    PB_FIELD(  4, MESSAGE , SINGULAR, POINTER , OTHER, TextLayerPropsMessage, layerProps, text, &LayerPropsMessage_fields),
    PB_FIELD(  5, BOOL    , SINGULAR, STATIC  , OTHER, TextLayerPropsMessage, alignmentChanged, layerProps, 0),
    PB_FIELD(  6, BOOL    , SINGULAR, STATIC  , OTHER, TextLayerPropsMessage, colorChanged, alignmentChanged, 0),
    PB_FIELD(  7, BOOL    , SINGULAR, STATIC  , OTHER, TextLayerPropsMessage, textChanged, colorChanged, 0),
    PB_FIELD(  8, BOOL    , SINGULAR, STATIC  , OTHER, TextLayerPropsMessage, layerPropsChanged, textChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t ArcLayerPropsMessage_fields[17] = {
    PB_FIELD(  1, INT32   , SINGULAR, STATIC  , FIRST, ArcLayerPropsMessage, endAngle, endAngle, 0),
    PB_FIELD(  2, INT32   , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, startAngle, endAngle, 0),
    PB_FIELD(  3, MESSAGE , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, center, startAngle, &PointMessage_fields),
    PB_FIELD(  4, UINT32  , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, color, center, 0),
    PB_FIELD(  5, UINT32  , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, radius, color, 0),
    PB_FIELD(  6, UINT32  , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, strokeColor, radius, 0),
    PB_FIELD(  7, UINT32  , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, strokeWidth, strokeColor, 0),
    PB_FIELD(  8, UINT32  , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, thickness, strokeWidth, 0),
    PB_FIELD(  9, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, centerChanged, thickness, 0),
    PB_FIELD( 10, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, colorChanged, centerChanged, 0),
    PB_FIELD( 11, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, endAngleChanged, colorChanged, 0),
    PB_FIELD( 12, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, radiusChanged, endAngleChanged, 0),
    PB_FIELD( 13, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, startAngleChanged, radiusChanged, 0),
    PB_FIELD( 14, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, strokeColorChanged, startAngleChanged, 0),
    PB_FIELD( 15, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, strokeWidthChanged, strokeColorChanged, 0),
    PB_FIELD( 16, BOOL    , SINGULAR, STATIC  , OTHER, ArcLayerPropsMessage, thicknessChanged, strokeWidthChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t PointMessage_fields[5] = {
    PB_FIELD(  1, INT32   , SINGULAR, STATIC  , FIRST, PointMessage, x, x, 0),
    PB_FIELD(  2, INT32   , SINGULAR, STATIC  , OTHER, PointMessage, y, x, 0),
    PB_FIELD(  3, BOOL    , SINGULAR, STATIC  , OTHER, PointMessage, xChanged, y, 0),
    PB_FIELD(  4, BOOL    , SINGULAR, STATIC  , OTHER, PointMessage, yChanged, xChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t PathLayerPropsMessage_fields[11] = {
    PB_FIELD(  1, INT32   , SINGULAR, STATIC  , FIRST, PathLayerPropsMessage, rotationAngle, rotationAngle, 0),
    PB_FIELD(  2, MESSAGE , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, rotationPoint, rotationAngle, &PointMessage_fields),
    PB_FIELD(  3, MESSAGE , REPEATED, POINTER , OTHER, PathLayerPropsMessage, points, rotationPoint, &PointMessage_fields),
    PB_FIELD(  4, UINT32  , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, fillColor, points, 0),
    PB_FIELD(  5, UINT32  , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, strokeColor, fillColor, 0),
    PB_FIELD(  6, BOOL    , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, fillColorChanged, strokeColor, 0),
    PB_FIELD(  7, BOOL    , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, pointsChanged, fillColorChanged, 0),
    PB_FIELD(  8, BOOL    , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, rotationAngleChanged, pointsChanged, 0),
    PB_FIELD(  9, BOOL    , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, rotationPointChanged, rotationAngleChanged, 0),
    PB_FIELD( 10, BOOL    , SINGULAR, STATIC  , OTHER, PathLayerPropsMessage, strokeColorChanged, rotationPointChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t CircleLayerPropsMessage_fields[5] = {
    PB_FIELD(  1, UINT32  , SINGULAR, STATIC  , FIRST, CircleLayerPropsMessage, radius, radius, 0),
    PB_FIELD(  2, MESSAGE , SINGULAR, POINTER , OTHER, CircleLayerPropsMessage, layerProps, radius, &LayerPropsMessage_fields),
    PB_FIELD(  3, BOOL    , SINGULAR, STATIC  , OTHER, CircleLayerPropsMessage, radiusChanged, layerProps, 0),
    PB_FIELD(  4, BOOL    , SINGULAR, STATIC  , OTHER, CircleLayerPropsMessage, layerPropsChanged, radiusChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t AnimationPropsMessage_fields[11] = {
    PB_FIELD(  1, BOOL    , SINGULAR, STATIC  , FIRST, AnimationPropsMessage, loop, loop, 0),
    PB_FIELD(  2, BOOL    , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, sequence, loop, 0),
    PB_FIELD(  3, MESSAGE , REPEATED, POINTER , OTHER, AnimationPropsMessage, endOperations, sequence, &OperationMessage_fields),
    PB_FIELD(  4, MESSAGE , REPEATED, POINTER , OTHER, AnimationPropsMessage, startOperations, endOperations, &OperationMessage_fields),
    PB_FIELD(  5, UINT32  , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, delay, startOperations, 0),
    PB_FIELD(  6, UINT32  , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, duration, delay, 0),
    PB_FIELD(  7, BOOL    , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, durationChanged, duration, 0),
    PB_FIELD(  8, BOOL    , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, delayChanged, durationChanged, 0),
    PB_FIELD(  9, BOOL    , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, loopChanged, delayChanged, 0),
    PB_FIELD( 10, BOOL    , SINGULAR, STATIC  , OTHER, AnimationPropsMessage, sequenceChanged, loopChanged, 0),
    PB_LAST_FIELD
};

const pb_field_t OperationMessage_fields[11] = {
    PB_FIELD(  1, UINT32  , SINGULAR, STATIC  , FIRST, OperationMessage, operation, operation, 0),
    PB_FIELD(  2, UINT32  , SINGULAR, STATIC  , OTHER, OperationMessage, nodeType, operation, 0),
    PB_FIELD(  3, STRING  , SINGULAR, POINTER , OTHER, OperationMessage, nodeId, nodeType, 0),
    PB_FIELD(  4, STRING  , SINGULAR, POINTER , OTHER, OperationMessage, parentNodeId, nodeId, 0),
    PB_FIELD(  5, MESSAGE , SINGULAR, POINTER , OTHER, OperationMessage, textLayerProps, parentNodeId, &TextLayerPropsMessage_fields),
    PB_FIELD(  6, MESSAGE , SINGULAR, POINTER , OTHER, OperationMessage, animationProps, textLayerProps, &AnimationPropsMessage_fields),
    PB_FIELD(  7, MESSAGE , SINGULAR, POINTER , OTHER, OperationMessage, layerProps, animationProps, &LayerPropsMessage_fields),
    PB_FIELD(  8, MESSAGE , SINGULAR, POINTER , OTHER, OperationMessage, circleLayerProps, layerProps, &CircleLayerPropsMessage_fields),
    PB_FIELD(  9, MESSAGE , SINGULAR, POINTER , OTHER, OperationMessage, arcLayerProps, circleLayerProps, &ArcLayerPropsMessage_fields),
    PB_FIELD( 10, MESSAGE , SINGULAR, POINTER , OTHER, OperationMessage, pathLayerProps, arcLayerProps, &PathLayerPropsMessage_fields),
    PB_LAST_FIELD
};

const pb_field_t BatchOperationsMessage_fields[2] = {
    PB_FIELD(  1, MESSAGE , REPEATED, POINTER , FIRST, BatchOperationsMessage, operations, operations, &OperationMessage_fields),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(ArcLayerPropsMessage, center) < 65536 && pb_membersize(PathLayerPropsMessage, rotationPoint) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_LayerPropsMessage_TextLayerPropsMessage_ArcLayerPropsMessage_PointMessage_PathLayerPropsMessage_CircleLayerPropsMessage_AnimationPropsMessage_OperationMessage_BatchOperationsMessage)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(ArcLayerPropsMessage, center) < 256 && pb_membersize(PathLayerPropsMessage, rotationPoint) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_LayerPropsMessage_TextLayerPropsMessage_ArcLayerPropsMessage_PointMessage_PathLayerPropsMessage_CircleLayerPropsMessage_AnimationPropsMessage_OperationMessage_BatchOperationsMessage)
#endif


/* @@protoc_insertion_point(eof) */
