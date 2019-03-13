import React, { Component, Fragment } from 'react';
import { GaugeChart } from '@pebble-react/components';

class App extends Component {
    state = {
        value: Math.floor(Math.random() * 101)
    };

    componentDidMount() {
        Pebble.addEventListener(
            'appmessage',
            event => {
                const { value } = this.state;

                this.setState(
                    {
                        value: value + 5
                    }
                );
            }
        );
    }

    render() {
        const { value } = this.state;

        return (
            <Fragment>
                <GaugeChart left={8} thickness={15} top={10} width={60} value={value} showPointer={false} />

                <GaugeChart left={76} thickness={15} top={10} width={60} value={100 - value} showPointer={false} />

                <GaugeChart left={8} thickness={20} top={60} width={128} value={value / 2} />
            </Fragment>
        );
    }
}

export default App;