import React, { Component, Fragment } from 'react';

class App extends Component {
    state = { show: true };

    componentDidMount() {
        setTimeout(
            () => {
                this.setState({ show: false });
            },
            2000
        );
    }

    render() {
        const { show } = this.state;

        return (
            show && (
                <Fragment>
                    <text
                        height={30}
                        left={0}
                        top={0}
                        width={130}
                    >
                        {'Text Memory'}
                    </text>
                </Fragment>
            )
        );
    }
}

export default App;