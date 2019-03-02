import PebbleComponent from './PebbleComponent';

import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { LayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class Layer extends PebbleComponent {
    getPropsMessage(props) {
        return {
            layerProps: LayerPropsMessage.create(
                {
                    ...props,
                    ...this.getPropsDiff(props),
                }
            )
        }
    }
}

export default Layer;