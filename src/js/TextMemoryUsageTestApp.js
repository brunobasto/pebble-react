import React, { Component, Fragment } from 'react';

class App extends Component {
    state = { show: true, top: 0 };

    componentDidMount() {
        // setTimeout(
        //     () => {
        //         this.setState({ show: false });
        //     },
        //     2000
        // );
        Pebble.addEventListener("appmessage", (e) => {
            this.setState({top: 50});
        });
    }

    render() {
        const { show, top } = this.state;

        return (
            show && (
                <Fragment>
                    <text
                        alignment={'center'}
                        height={30}
                        left={0}
                        top={top}
                        width={130}
                    >
                        {'Memory'}
                    </text>
                </Fragment>
            )
        );
    }
}

export default App;