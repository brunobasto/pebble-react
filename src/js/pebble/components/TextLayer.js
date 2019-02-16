import Layer from './Layer';

class TextLayer extends Layer {
    get children() {
        return this.props.children;
    }

    shouldSerializeProp() {
        return true;
    }
}

export default TextLayer;