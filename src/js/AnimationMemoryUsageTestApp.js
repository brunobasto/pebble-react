import React, { Component, Fragment } from 'react';

class App extends Component {
    state = { show: true, what: 'React' };

    render() {
        return (
            <animation
                animationProps={{
                    top: {
                        start: -25,
                        end: 70
                    }
                }}
                duration={1500}
            >
                <text
                    alignment="left"
                    height={30}
                    left={0}
                    top={-25}
                    width={130}
                >
                    {'Memory'}
                </text>

                <text
                    alignment="right"
                    height={30}
                    left={70}
                    top={-25}
                    width={130}
                >
                    {'Memory 2'}
                </text>
            </animation>
        );
    }
}

export default App;