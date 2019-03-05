import Reconciler from 'react-reconciler';
import { createElement } from '../utils/createElement';
import TextNode from '../components/TextNode';
import { flushMessages } from '../utils/messaging'

export let ROOT_NODE = {};

const DEFAULT_HOST_CONTEXT = {};

const PebbleRenderer = Reconciler({
    appendChild(parent, child) {
        if (parent.appendChild) {
            parent.appendChild(child);
        }
        if (typeof child.render === 'function') child.render(parent); // we just added a new child, so we want to render it
    },

    appendInitialChild(parent, child) {
        if (parent.appendChild) {
            parent.appendChild(child);
        }
    },

    appendChildToContainer(parent, child) {
        parent.appendChild(child);
    },

    removeChild(parent, child) {
        parent.removeChild(child);
    },

    removeChildFromContainer(parent, child) {
        parent.removeChild(child);
    },

    createInstance(type, props, internalInstanceHandle) {
        return createElement(type, props, internalInstanceHandle);
    },

    createTextInstance(text, rootContainerInstance, internalInstanceHandle) {
        return new TextNode(text);
    },

    commitTextUpdate(textInstance, oldText, newText) {
        textInstance.commitUpdate(newText);
    },

    insertInContainerBefore(parent, child, beforeChild) {
        parent.insertBefore(child, beforeChild);
    },

    insertBefore(parent, child, beforeChild) {
        parent.insertBefore(child, beforeChild);
    },

    finalizeInitialChildren(element, type, props) {
        return true;
    },

    getPublicInstance(inst) {
        return inst;
    },

    prepareForCommit() {
    },

    prepareUpdate(component, type, oldProps, newProps) {
        return component.prepareUpdate(oldProps, newProps, ROOT_NODE);
    },

    commitMount(component, type, newProps, internalInstanceHandle) {
    },

    commitUpdate(
        component,
        updatePayload,
        type,
        oldProps,
        newProps,
        internalInstanceHandle
    ) {
        const changed = {};

        for (const key in newProps) {
            if (oldProps[key] !== newProps[key]) {
                changed[key] = newProps[key];
            }
        }

        component.commitUpdate(changed);
    },

    resetAfterCommit() {
        flushMessages();
    },

    resetTextContent(wordElement) {
    },

    getRootHostContext(rootInstance) {
        return DEFAULT_HOST_CONTEXT;
    },

    getChildHostContext(parentHostContext) {
        return parentHostContext;
    },

    shouldSetTextContent(type, props) {
        return false;
    },

    now: () => { },

    supportsMutation: true,
    supportsPersistence: false
});

// renders the component
async function render(element) {
    ROOT_NODE = createElement('ROOT');

    const node = PebbleRenderer.createContainer(ROOT_NODE);

    PebbleRenderer.updateContainer(element, node, null);
}

export default render;