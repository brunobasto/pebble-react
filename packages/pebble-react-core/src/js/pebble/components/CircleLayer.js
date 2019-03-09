import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { CircleLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class CircleLayer extends Layer {
    getPropsMessage(props) {
        const diff = this.getPropsDiff(props);

        return {
            circleLayerProps: CircleLayerPropsMessage.create(
                {
                    ...props,
                    ...super.getPropsMessage(
                        {
                            ...props
                        }
                    ),
                    ...diff,
                    layerPropsChanged: true
                }
            )
        }
    }
}

export default CircleLayer;