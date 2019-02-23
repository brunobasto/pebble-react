import PebbleComponent from './PebbleComponent';
import Layer from './Layer';
import { runInThisContext } from 'vm';

class Animation extends PebbleComponent {
    get animationProps() {
        const {animationProps} = this.props;

        if (this.isSequence()) {
            return animationProps;
        }

        const propsArray = [];

        for (let key in animationProps) {
            propsArray.push(
                {
                    end: animationProps[key].end,
                    start: animationProps[key].start
                }
            );
        }

        return propsArray;
    }

    getPropsMessage() {
        // for each children, create a start and end operation
        // containing the getPropsMessage of each
        // the "props" parameter sent to them should be collected from
        // the animationProps array
    }

    isSequence() {
        return Array.isArray(this.props.animationProps) ? 1 : 0;
    }

    remove() {
        this._children.forEach(element => {
            this.removeChild(element);
        });
    }

    shouldSerializeProp() {
        return true;
    }
}

export default Animation;