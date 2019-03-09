import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { ArcLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class ArcLayer extends Layer {
    getPropsMessage(props) {
        const diff = this.getPropsDiff(props);

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
                    layerPropsChanged: true
                }
            )
        }
    }
}

export default ArcLayer;