import PebbleComponent from './PebbleComponent';
import {sendPebbleMessage} from '../utils/pebbleMessaging'
import {Operations, NodeTypes} from '../utils/constants';

class TextLayer extends PebbleComponent {
    static NODE_TYPE = NodeTypes.textLayer;

    appendInitialChild() {
        sendPebbleMessage(
            {
                operation: Operations.appendInitialChild,
                nodeType: NodeTypes.textLayer,
                nodeId: this.uniqueId,
                props: this.getSerializedProps()
            }
        );
    }

    getSerializedProps() {
        const {props} = this;

        return JSON.stringify(
            {
                height: `${props.height}`,
                left: `${props.left}`,
                text: props.children,
                top: `${props.top}`,
                width: `${props.width}`,
            }
        );
    }

    commitUpdate(newProps) {
        sendPebbleMessage(
            {
                operation: Operations.commitUpdate,
                nodeType: NodeTypes.textLayer,
                nodeId: this.uniqueId,
                props: this.getSerializedProps()
            }
        );
    }
}

export default TextLayer;