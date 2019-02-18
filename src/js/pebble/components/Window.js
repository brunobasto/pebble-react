import PebbleComponent from './PebbleComponent';

class Window extends PebbleComponent {
    getInternalProps() {
        return {
            test: `test`,
        };
    }
}

export default Window;