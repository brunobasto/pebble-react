import { Operations } from '../utils/constants';
import { enqueueMessage } from '../utils/messaging'

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
                {
                    operation: Operations.appendChild,
                    nodeType: child.nodeType,
                    nodeId: child.uniqueId,
                    props: this.getPropsAfterGetters(child, child.props)
                }
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

            child.remove();
        }

        this._children = this._children.filter(c => c !== child);
    }

    remove() {

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
            {
                operation: Operations.commitUpdate,
                nodeType: this.nodeType,
                nodeId: this.uniqueId,
                props: this.getPropsAfterGetters(this, newProps)
            }
        );
    }

    hasGetter(component, prop) {
        let proto = Object.getPrototypeOf(component);

        while (proto != null) {
            let descriptor = Object.getOwnPropertyDescriptor(proto, prop);

            if (descriptor && descriptor.get) {
                return true;
            }

            proto = Object.getPrototypeOf(proto);
        }

        return false;
    }

    shouldSerializeProp(prop) {
        if (prop === 'children') {
            return false;
        }

        return true;
    }

    getInternalProps() {
        return {};
    }

    getPropsAfterGetters(component, props) {
        const serializedProps = {};

        for (const key in props) {
            if (component.shouldSerializeProp(key) === false) {
                continue;
            }

            if (this.hasGetter(component, key)) {
                serializedProps[key] = component[key];
            }
            else {
                serializedProps[key] = props[key];
            }
        }

        return {
            ...serializedProps,
            ...component.getInternalProps()
        };
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