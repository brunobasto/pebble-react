import React, { Component, Fragment } from 'react';

class App extends Component {
    state = { show: true, what: 'React' };

    componentDidMount() {
        Pebble.addEventListener(
            'appmessage',
            (e) => {
                this.setState(
                    {
                        what: 'Peble'
                    }
                );
            }
        );
    }

    render() {
        const { what } = this.state;

        return (
            <text
                alignment={'center'}
                height={30}
                left={0 * 29}
                top={50}
                width={129}
            >
                {what}
            </text>
        );
    }
}

export default App;