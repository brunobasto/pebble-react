
è
BatchOperationsMessage.protonanopb.proto"
LayerPropsMessage
height (B’?8Rheight
left (B’?8Rleft
top (B’?8Rtop
width (B’?8Rwidth$
heightChanged (RheightChanged 
leftChanged (RleftChanged

topChanged (R
topChanged"
widthChanged (RwidthChanged"È
TextLayerPropsMessage#
	alignment (	B’?R	alignment
color (Rcolor
text (	B’?Rtext9

layerProps (2.LayerPropsMessageB’?R
layerProps*
alignmentChanged (RalignmentChanged"
colorChanged (RcolorChanged 
textChanged (RtextChanged,
layerPropsChanged (RlayerPropsChanged"€
ArcLayerPropsMessage!
endAngle (B’?8RendAngle%

startAngle (B’?8R
startAngle%
center (2.PointMessageRcenter
color (Rcolor
radius (B’?8Rradius 
strokeColor (RstrokeColor'
strokeWidth (B’?8RstrokeWidth#
	thickness (B’?8R	thickness$
centerChanged	 (RcenterChanged"
colorChanged
 (RcolorChanged(
endAngleChanged (RendAngleChanged$
radiusChanged (RradiusChanged,
startAngleChanged (RstartAngleChanged.
strokeColorChanged (RstrokeColorChanged.
strokeWidthChanged (RstrokeWidthChanged*
thicknessChanged (RthicknessChanged"p
PointMessage
x (B’?8Rx
y (B’?8Ry
xChanged (RxChanged
yChanged (RyChanged"Ñ
PathLayerPropsMessage+
rotationAngle (B’?8RrotationAngle3
rotationPoint (2.PointMessageRrotationPoint,
points (2.PointMessageB’?Rpoints
	fillColor (R	fillColor 
strokeColor (RstrokeColor*
fillColorChanged (RfillColorChanged$
pointsChanged (RpointsChanged2
rotationAngleChanged (RrotationAngleChanged2
rotationPointChanged	 (RrotationPointChanged.
strokeColorChanged
 (RstrokeColorChanged"Ç
CircleLayerPropsMessage
radius (B’?8Rradius9

layerProps (2.LayerPropsMessageB’?R
layerProps$
radiusChanged (RradiusChanged,
layerPropsChanged (RlayerPropsChanged"¥
AnimationPropsMessage
loop (Rloop
sequence (Rsequence>
endOperations (2.OperationMessageB’?RendOperationsB
startOperations (2.OperationMessageB’?RstartOperations
delay (B’?8Rdelay!
duration (B’?8Rduration(
durationChanged (RdurationChanged"
delayChanged (RdelayChanged 
loopChanged	 (RloopChanged(
sequenceChanged
 (RsequenceChanged"Å
OperationMessage#
	operation (B’?8R	operation!
nodeType (B’?8RnodeType
nodeId (	B’?RnodeId)
parentNodeId (	B’?RparentNodeIdE
textLayerProps (2.TextLayerPropsMessageB’?RtextLayerPropsE
animationProps (2.AnimationPropsMessageB’?RanimationProps9

layerProps (2.LayerPropsMessageB’?R
layerPropsK
circleLayerProps (2.CircleLayerPropsMessageB’?RcircleLayerPropsB
arcLayerProps	 (2.ArcLayerPropsMessageB’?RarcLayerPropsE
pathLayerProps
 (2.PathLayerPropsMessageB’?RpathLayerProps"R
BatchOperationsMessage8

operations (2.OperationMessageB’?R
operationsbproto3