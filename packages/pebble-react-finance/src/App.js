import React, { Component, Fragment } from 'react';
import { Badge, GaugeChart } from '@pebble-react/components';

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
                <Badge
                    borderRadius={7}
                    left={10}
                    top={10}
                >
                    <text top={9} height={22} width={144}>Expenses</text>
                </Badge>
                <GaugeChart
                    left={8}
                    thickness={20}
                    top={50}
                    value={value}
                    width={128}
                />
                <text top={130} height={22} width={144}>R$ 2.565,87</text>
            </Fragment>
        );
    }
}

export default App;