import { enqueueMessage } from '../utils/messaging'
import { Operations } from '../utils/constants';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { OperationMessage } = protobuf.Root.fromJSON(protoJSON);

let idCounter = 0;

class PebbleComponent {
    constructor(props, nodeType) {
        this.props = props;
        this.uniqueId = `${nodeType}${++idCounter}`;
        this.nodeType = nodeType;
        this._children = [];
    }

    appendChild(child) {
        if (child instanceof PebbleComponent) {
            enqueueMessage(
                OperationMessage.create(
                    {
                        ...child.getPropsMessage(child.props),
                        operation: Operations.appendChild,
                        nodeType: child.nodeType,
                        nodeId: child.uniqueId
                    }
                )
            );
        }

        this._children.push(child);
    }

    removeChild(child) {
        if (child instanceof PebbleComponent) {
            enqueueMessage(
                {
                    operation: Operations.removeChild,
                    nodeType: child.nodeType,
                    nodeId: child.uniqueId
                }
            );

            child._children.forEach(c => child.removeChild(c));
        }

        this._children = this._children.filter(c => c !== child);
    }

    prepareUpdate(oldProps, newProps) {
        for (const key in newProps) {
            if (oldProps[key] !== newProps[key]) {
                return true;
            }
        }

        return false;
    }

    commitUpdate(newProps) {
        this.props = {
            ...this.props,
            ...newProps
        };

        enqueueMessage(
            OperationMessage.create(
                {
                    ...this.getPropsMessage(newProps),
                    operation: Operations.commitUpdate,
                    nodeType: this.nodeType,
                    nodeId: this.uniqueId
                }
            )
        );
    }

    getPropsDiff(props) {
        const diff = {};

        for (const name in props) {
            diff[`${name}Changed`] = 1;
        }

        return diff;
    }

    getPropsMessage() {
        throw new Error('Every component should implement getPropsMessage(props)');
    }

    insertBefore(child) {
        this.appendChild(child);
    }

    getPublicInstance() {
        return this;
    }

    finalizeBeforeMount(type, props, rootContainerInstance) {
        return false;
    }

    finalizeBeforeRemoval() { }

    commitMount(newProps) { }

    update() { }

}

export default PebbleComponent;