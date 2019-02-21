/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.2 at Thu Feb 21 13:45:12 2019. */

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
typedef struct _TextLayerPropsMessage {
    char alignment[10];
    uint8_t height;
    int16_t left;
    char text[10];
    int16_t top;
    uint8_t width;
    bool alignmentChanged;
    bool heightChanged;
    bool leftChanged;
    bool textChanged;
    bool topChanged;
    bool widthChanged;
/* @@protoc_insertion_point(struct:TextLayerPropsMessage) */
} TextLayerPropsMessage;

typedef struct _OperationMessage {
    uint8_t operation;
    uint8_t nodeType;
    char nodeId[5];
    TextLayerPropsMessage textLayerProps;
/* @@protoc_insertion_point(struct:OperationMessage) */
} OperationMessage;

typedef struct _BatchOperationsMessage {
    pb_size_t operations_count;
    OperationMessage operations[2];
/* @@protoc_insertion_point(struct:BatchOperationsMessage) */
} BatchOperationsMessage;

/* Default values for struct fields */

/* Initializer values for message structs */
#define TextLayerPropsMessage_init_default       {"", 0, 0, "", 0, 0, 0, 0, 0, 0, 0, 0}
#define OperationMessage_init_default            {0, 0, "", TextLayerPropsMessage_init_default}
#define BatchOperationsMessage_init_default      {0, {OperationMessage_init_default, OperationMessage_init_default}}
#define TextLayerPropsMessage_init_zero          {"", 0, 0, "", 0, 0, 0, 0, 0, 0, 0, 0}
#define OperationMessage_init_zero               {0, 0, "", TextLayerPropsMessage_init_zero}
#define BatchOperationsMessage_init_zero         {0, {OperationMessage_init_zero, OperationMessage_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define TextLayerPropsMessage_alignment_tag      1
#define TextLayerPropsMessage_height_tag         2
#define TextLayerPropsMessage_left_tag           3
#define TextLayerPropsMessage_text_tag           4
#define TextLayerPropsMessage_top_tag            5
#define TextLayerPropsMessage_width_tag          6
#define TextLayerPropsMessage_alignmentChanged_tag 7
#define TextLayerPropsMessage_heightChanged_tag  8
#define TextLayerPropsMessage_leftChanged_tag    9
#define TextLayerPropsMessage_textChanged_tag    10
#define TextLayerPropsMessage_topChanged_tag     11
#define TextLayerPropsMessage_widthChanged_tag   12
#define OperationMessage_operation_tag           1
#define OperationMessage_nodeType_tag            2
#define OperationMessage_nodeId_tag              3
#define OperationMessage_textLayerProps_tag      4
#define BatchOperationsMessage_operations_tag    1

/* Struct field encoding specification for nanopb */
extern const pb_field_t TextLayerPropsMessage_fields[13];
extern const pb_field_t OperationMessage_fields[5];
extern const pb_field_t BatchOperationsMessage_fields[2];

/* Maximum encoded size of messages (where known) */
#define TextLayerPropsMessage_size               70
#define OperationMessage_size                    91
#define BatchOperationsMessage_size              186

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define BATCHOPERATIONSMESSAGE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
