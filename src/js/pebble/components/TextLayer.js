import Layer from './Layer';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { TextLayerPropsMessage } = protobuf.Root.fromJSON(protoJSON);

class TextLayer extends Layer {
    get children() {
        return this.props.children;
    }

    getPropsMessage(props) {
        return {
            textLayerProps: TextLayerPropsMessage.create(
                {
                    ...props,
                    alignmentChanged: props.hasOwnProperty('alignment'),
                    heightChanged: props.hasOwnProperty('height'),
                    leftChanged: props.hasOwnProperty('left'),
                    text: props.children,
                    textChanged: props.hasOwnProperty('children'),
                    topChanged: props.hasOwnProperty('top'),
                    widthChanged: props.hasOwnProperty('width')
                }
            )
        }
    }

    shouldSerializeProp() {
        return true;
    }
}

export default TextLayer;