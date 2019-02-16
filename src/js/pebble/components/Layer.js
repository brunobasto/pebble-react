import PebbleComponent from './PebbleComponent';

class Layer extends PebbleComponent {
    get top() {
        return `${this.props.top}`;
    }

    get left() {
        return `${this.props.left}`;
    }

    get width() {
        return `${this.props.width}`;
    }

    get height() {
        return `${this.props.height}`;
    }
}

export default Layer;