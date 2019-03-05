import {
    Root,
    Animation,
    CircleLayer,
    ImageLayer,
    TextLayer,
    Window
} from '../components';
import { NodeTypes } from './constants';

/**
 * Creates an element for a document
 * @param {string} type Element type
 * @param {Object} props Component props
 * @param {Object} root Root instance
 */
function createElement(type, props) {
    const COMPONENTS = {
        'window': () => new Window(props, NodeTypes.windowLayer),
        'circle': () => new CircleLayer(props, NodeTypes.circleLayer),
        'text': () => new TextLayer(props, NodeTypes.textLayer),
        'image': () => new ImageLayer(props, NodeTypes.imageLayer),
        'animation': () => new Animation(props, NodeTypes.animation),
        ROOT: () => new Root({}, NodeTypes.rootLayer),
        default: undefined
    }

    return COMPONENTS[type]() || COMPONENTS.default;
}

export { createElement };