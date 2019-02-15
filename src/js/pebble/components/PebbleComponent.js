import {Operations} from '../utils/constants';
import {sendPebbleMessage} from '../utils/pebbleMessaging'

let idCounter = 0;

class PebbleComponent {
    constructor(props, nodeType) {
        this.props = props;
        this.uniqueId = `${nodeType}${++idCounter}`;
        this.nodeType = nodeType;
    }

    appendChild(child) {
        // child.uniqueId = `${child.NODE_TYPE}${++idCounter}`;
        // console.log('PebbleComponent appendChild called for', child.uniqueId);
    }

    insertBefore() { }

    getPublicInstance() {
        return this;
    }

    removeChild(child) {
        sendPebbleMessage(
            {
                operation: Operations.removeChild,
                nodeType: child.nodeType,
                nodeId: child.uniqueId
            }
        );
    }

    prepareUpdate(oldProps, newProps, rootContainerInstance) {
        this.props = newProps;

        return true;
    }

    commitUpdate(newProps) {
    }

    appendInitialChild(child) { }

    finalizeBeforeMount(type, props, rootContainerInstance) {
        return false;
    }

    finalizeBeforeRemoval() { }

    commitMount(newProps) { }

    update() { }

}

export default PebbleComponent;