import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { TextLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class TextLayer extends Layer {
    getPropsMessage(props) {
        const diff = this.getPropsDiff(props);

        return {
            textLayerProps: TextLayerPropsMessage.create(
                {
                    ...props,
                    ...super.getPropsMessage(props),
                    ...diff,
                    layerPropsChanged: true,
                    text: props.children,
                    textChanged: diff.childrenChanged
                }
            )
        }
    }
}

export default TextLayer;