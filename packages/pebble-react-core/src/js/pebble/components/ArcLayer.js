import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { ArcLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class ArcLayer extends Layer {
    getPropsMessage(props) {
        const diff = this.getPropsDiff(props);

        const color = parseInt((props.color || '#FFFFFF').replace('#', ''), 16);

        return {
            arcLayerProps: ArcLayerPropsMessage.create(
                {
                    ...props,
                    ...super.getPropsMessage(
                        {
                            ...props
                        }
                    ),
                    ...diff,
                    color: props.color ? color : undefined,
                    layerPropsChanged: true
                }
            )
        }
    }
}

export default ArcLayer;