import PebbleComponent from './PebbleComponent';
import protobuf from 'protobufjs';
import protoJSON from './../../proto.json'

const { PathLayerPropsMessage, PointMessage } = protobuf.Root.fromJSON(protoJSON);

class PathLayer extends PebbleComponent {
    getPropsMessage(props) {
        const { points } = props;
        const diff = this.getPropsDiff(props);
        const fillColor = parseInt((props.fillColor || '#FFFFFF').replace('#', ''), 16);
        const strokeColor = parseInt((props.strokeColor || '#FFFFFF').replace('#', ''), 16);

        return {
            pathLayerProps: PathLayerPropsMessage.create(
                {
                    ...props,
                    ...diff,
                    fillColor: props.fillColor ? fillColor : undefined,
                    points: this.getPointsMessage(points),
                    rotationPoint: this.getRotationPoint(props.rotationPoint),
                    strokeColor: props.strokeColor ? strokeColor : undefined,
                }
            )
        }
    }

    getPointsMessage(points) {
        if (!points) {
            return undefined;
        }

        return points.map(point => PointMessage.create(point));
    }

    getRotationPoint(point) {
        if (!point) {
            return undefined;
        }

        return PointMessage.create(point);
    }
}

export default PathLayer;