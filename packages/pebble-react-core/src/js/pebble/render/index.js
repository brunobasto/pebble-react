import { createElement } from '../utils/createElement';
import { flushMessages } from '../utils/messaging'
import Reconciler from 'react-reconciler';
import TextNode from '../components/TextNode';

import * as NoPersistence from './NoPersistence';
import * as NoHydration from './NoHydration';

import {
    now as FrameSchedulingNow,
    cancelDeferredCallback as FrameSchedulingCancelDeferredCallback,
    scheduleDeferredCallback as FrameSchedulingScheduleDeferredCallback,
    shouldYield as FrameSchedulingShouldYield,
  } from './Scheduling';

export let ROOT_NODE = {};

const DEFAULT_HOST_CONTEXT = {};

const PebbleRenderer = Reconciler({
    // ...NoHydration,
    // ...NoPersistence,

    now: FrameSchedulingNow,
    isPrimaryRenderer: true,
    scheduleDeferredCallback: FrameSchedulingScheduleDeferredCallback,
    cancelDeferredCallback: FrameSchedulingCancelDeferredCallback,
    shouldYield: FrameSchedulingShouldYield,
    scheduleTimeout: setTimeout,
    cancelTimeout: clearTimeout,
    noTimeout: -1,
    schedulePassiveEffects: FrameSchedulingScheduleDeferredCallback,
    cancelPassiveEffects: FrameSchedulingCancelDeferredCallback,

    appendInitialChild(parent, child) {
        if (parent.appendChild) {
            parent.appendChild(child);
        }
    },

    createInstance(type, props, internalInstanceHandle) {
        if (typeof type === 'function') {
            return type(props, internalInstanceHandle);
        }

        return createElement(type, props, internalInstanceHandle);
    },

    createTextInstance(text, rootContainerInstance, internalInstanceHandle) {
        return new TextNode(text);
    },

    finalizeInitialChildren(element, type, props) {
        return true;
    },

    getRootHostContext(rootInstance) {
        return rootInstance;
    },

    getChildHostContext(parentHostContext) {
        return parentHostContext;
    },

    getPublicInstance(inst) {
        return inst;
    },

    prepareForCommit() {
    },

    prepareUpdate(component, type, oldProps, newProps) {
        return component.prepareUpdate(oldProps, newProps, ROOT_NODE);
    },

    resetAfterCommit() {
        flushMessages();
    },

    shouldDeprioritizeSubtree(type, props) {
        return false;
    },

    shouldSetTextContent(props) {
        return false;
    },

    // Mutation

    appendChild(parent, child) {
        if (parent.appendChild) {
            parent.appendChild(child);
        }
        if (typeof child.render === 'function') child.render(parent); // we just added a new child, so we want to render it
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

    commitTextUpdate(textInstance, oldText, newText) {
        textInstance.commitUpdate(newText);
    },

    insertInContainerBefore(parent, child, beforeChild) {
        parent.insertBefore(child, beforeChild);
    },

    insertBefore(parent, child, beforeChild) {
        parent.insertBefore(child, beforeChild);
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

    resetTextContent(wordElement) {
    },

    shouldSetTextContent(type, props) {
        return false;
    },

    supportsMutation: true,
});

// renders the component
function render(element) {
    ROOT_NODE = createElement('ROOT');

    const node = PebbleRenderer.createContainer(ROOT_NODE);

    PebbleRenderer.updateContainer(element, node, null);

    return PebbleRenderer.getPublicRootInstance(node);
}

export default render;