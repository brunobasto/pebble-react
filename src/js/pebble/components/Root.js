import PebbleComponent from './PebbleComponent';
import {NodeTypes} from '../utils/constants';

class Root extends PebbleComponent {
    static NODE_TYPE = NodeTypes.rootLayer;
}

export default Root;