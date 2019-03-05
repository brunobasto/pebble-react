import PebbleComponent from './PebbleComponent';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'
import { Operations } from '../utils/constants';

const { AnimationPropsMessage, OperationMessage } = protobuf.Root.fromJSON(protoJSON);

class Animation extends PebbleComponent {
    getAnimationPropsArray(props) {
        const { animationProps } = props;

        if (this.isSequence(animationProps)) {
            return animationProps;
        }

        const propsArray = [];

        for (let name in animationProps) {
            propsArray.push(
                {
                    name,
                    end: animationProps[name].end,
                    start: animationProps[name].start
                }
            );
        }

        return propsArray;
    }

    getPropsMessage(props) {
        const startOperations = this.getStartOperations(props);
        const endOperations = this.getEndOperations(props);

        delete props.animationProps;
        delete props.children;

        return {
            animationProps: AnimationPropsMessage.create(
                {
                    ...props,
                    startOperations,
                    endOperations,
                    delayChanged: props.hasOwnProperty('delay'),
                    durationChanged: props.hasOwnProperty('duration'),
                    loopChanged: props.hasOwnProperty('loop'),
                    sequenceChanged: props.hasOwnProperty('sequence'),
                }
            )
        };
    }

    getEndOperations(props) {
        const { _children } = this;
        const animationPropsArray = this.getAnimationPropsArray(props);

        return _children.map(
            child => {
                const props = {};

                animationPropsArray.forEach(
                    ({ name, end }) => {
                        props[name] = end;
                    }
                );

                return OperationMessage.create(
                    {
                        nodeId: child.uniqueId,
                        nodeType: child.nodeType,
                        operation: Operations.commitUpdate,
                        parentNodeId: this.uniqueId,
                        ...child.getPropsMessage(props)
                    }
                );
            }
        );
    }

    getStartOperations(props) {
        const { _children } = this;
        const animationPropsArray = this.getAnimationPropsArray(props);

        return _children.map(
            child => {
                const props = {};

                animationPropsArray.forEach(
                    ({ name, start }) => {
                        props[name] = start;
                    }
                );

                return OperationMessage.create(
                    {
                        nodeId: child.uniqueId,
                        nodeType: child.nodeType,
                        operation: Operations.commitUpdate,
                        parentNodeId: this.uniqueId,
                        ...child.getPropsMessage(props)
                    }
                );
            }
        );
    }

    isSequence(animationProps) {
        return Array.isArray(animationProps) ? 1 : 0;
    }
}

export default Animation;