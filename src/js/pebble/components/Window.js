import PebbleComponent from './PebbleComponent';
import {NodeTypes} from '../utils/constants';

class Window extends PebbleComponent {
    static NODE_TYPE = NodeTypes.windowLayer;
}

export default Window;