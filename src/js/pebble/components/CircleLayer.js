import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { CircleLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class CircleLayer extends Layer {
    getPropsMessage(props) {
        const {radius} = props;
        const diff = this.getPropsDiff(props);

        return {
            circleLayerProps: CircleLayerPropsMessage.create(
                {
                    ...props,
                    ...super.getPropsMessage(
                        {
                            ...props,
                            height: radius * 2,
                            width: radius * 2,
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