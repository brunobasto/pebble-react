import {
    Animation,
    ArcLayer,
    CircleLayer,
    ImageLayer,
    Layer,
    PathLayer,
    Root,
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
        'animation': () => new Animation(props, NodeTypes.animation),
        'arc': () => new ArcLayer(props, NodeTypes.arcLayer),
        'circle': () => new CircleLayer(props, NodeTypes.circleLayer),
        'image': () => new ImageLayer(props, NodeTypes.imageLayer),
        'layer': () => new Layer(props, NodeTypes.layer),
        'path': () => new PathLayer(props, NodeTypes.pathLayer),
        'text': () => new TextLayer(props, NodeTypes.textLayer),
        'window': () => new Window(props, NodeTypes.windowLayer),
        ROOT: () => new Root({}, NodeTypes.rootLayer),
        default: undefined
    }

    return COMPONENTS[type]() || COMPONENTS.default;
}

export { createElement };