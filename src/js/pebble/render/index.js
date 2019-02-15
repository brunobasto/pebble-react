import Reconciler from 'react-reconciler';
import { createElement } from '../utils/createElement';
import TextNode from '../components/TextNode';

export let ROOT_NODE = {};

const DEFAULT_HOST_CONTEXT = {};

const PebbleRenderer = Reconciler({
    appendChild(parentInstance, child) {
        console.log('Called reconciler appendChild', Object.keys(child));

        if (parentInstance.appendChild) {
            parentInstance.appendChild(child);
        }
        if (typeof child.render === 'function') child.render(parentInstance); // we just added a new child, so we want to render it
    },

    appendInitialChild(parentInstance, child) {
        console.log('appendInitialChild');
        if (parentInstance.appendChild) {
            parentInstance.appendInitialChild(child);
        }
    },

    removeChild(parentInstance, child) {
        parentInstance.removeChild(child);
    },

    appendChildToContainer(parentInstance, child) {
        console.log('appendChildToContainer');
        parentInstance.appendChild(child);
    },

    createInstance(type, props, internalInstanceHandle) {
        return createElement(type, props, internalInstanceHandle);
    },

    createTextInstance(text, rootContainerInstance, internalInstanceHandle) {
        console.log('createTextInstance', text);
        return new TextNode(text);
    },

    commitTextUpdate(textInstance, oldText, newText) {
        textInstance.commitUpdate(newText);
    },

    finalizeInitialChildren(element, type, props) {
        console.log('finalizeInitialChildren');
        return false;
    },

    getPublicInstance(inst) {
        return inst;
    },

    prepareForCommit() {
        // noop
        console.log('prepareForCommit');
    },

    prepareUpdate(component, type, oldProps, newProps) {
        return component.prepareUpdate(oldProps, newProps, ROOT_NODE);
    },

    commitMount(component, type, newProps, internalInstanceHandle) {
        console.log('commitMount');
    },

    commitUpdate(
        component,
        updatePayload,
        type,
        oldProps,
        newProps,
        internalInstanceHandle
    ) {
        component.commitUpdate(newProps);
    },

    resetAfterCommit() {
        // noop
        console.log('resetAfterCommit');
    },

    resetTextContent(wordElement) {
        // noop
        console.log('resetTextContent');
    },

    getRootHostContext(rootInstance) {
        console.log('getRootHostContext');

        return DEFAULT_HOST_CONTEXT;
    },

    getChildHostContext(parentHostContext) {
        console.log('getChildHostContext');

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

    // await sendPebbleRenderMessage({
    //     renderNodeType: node.type
    // });
}

export default render;