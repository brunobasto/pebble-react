import PebbleComponent from './PebbleComponent';

class Window extends PebbleComponent {
    getInternalProps() {
        return {
            test: `test`,
        };
    }

    getPropsMessage(props) {
        return {};
    }
}

export default Window;