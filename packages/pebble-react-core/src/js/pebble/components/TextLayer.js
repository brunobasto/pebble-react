import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { TextLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class TextLayer extends Layer {
    getPropsMessage(props) {
        const diff = this.getPropsDiff(props);

        const color = parseInt((props.color || '#FFFFFF').replace('#', ''), 16);

        return {
            textLayerProps: TextLayerPropsMessage.create(
                {
                    ...props,
                    ...super.getPropsMessage(props),
                    ...diff,
                    color: props.color ? color : undefined,
                    layerPropsChanged: true,
                    text: props.children,
                    textChanged: diff.childrenChanged
                }
            )
        }
    }
}

export default TextLayer;