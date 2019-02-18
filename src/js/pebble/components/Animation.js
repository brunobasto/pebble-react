import PebbleComponent from './PebbleComponent';
import Layer from './Layer';
import { runInThisContext } from 'vm';

class Animation extends PebbleComponent {
    get animationProps() {
        const {animationProps} = this.props;

        if (this.isSequence()) {
            return animationProps.map(p => {
                return {
                    ...p,
                    start: `${p.start}`,
                    end: `${p.end}`
                }
            });
        }

        const propsArray = [];

        for (let key in animationProps) {
            propsArray.push(
                {
                    end: `${animationProps[key].end}`,
                    name: key,
                    start: `${animationProps[key].start}`,
                    type: typeof animationProps[key].start
                }
            );
        }

        return propsArray;
    }

    get children() {
        return this._children.map(child => child.uniqueId);
    }

    get duration() {
        return `${this.props.duration}`;
    }

    get loop() {
        return `${this.props.loop ? 1 : 0}`;
    }

    isSequence() {
        return Array.isArray(this.props.animationProps) ? 1 : 0;
    }

    getInternalProps() {
        return {
            isSequence: `${this.isSequence()}`,
            animationsLength: `${this.animationProps.length}`,
            childrenLength: `${this._children.length}`
        };
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