import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { ArcLayerPropsMessage, PointMessage } = protobuf.Root.fromJSON(protoJSON);

class ArcLayer extends Layer {
    getPropsMessage(props) {
        const {center} = props;
        const color = parseInt((props.color || '#FFFFFF').replace('#', ''), 16);
        const diff = this.getPropsDiff(props);

        return {
            arcLayerProps: ArcLayerPropsMessage.create(
                {
                    ...props,
                    ...diff,
                    center: center ? PointMessage.create(center) : undefined,
                    color: props.color ? color : undefined
                }
            )
        }
    }
}

export default ArcLayer;