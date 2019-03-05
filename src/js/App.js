import React, { Component, Fragment } from 'react';

const letterWidth = 29;

class App extends Component {
    state = {
        what: 'React'
    };

    componentDidMount() {
        Pebble.addEventListener(
            'appmessage',
            event => {
                this.setState(
                    {
                        what: 'Pebble'
                    }
                );
            }
        );
    }

    render() {
        const {what} = this.state;

        return (
            <Fragment>
                <animation
                    animationProps={{
                        radius: {
                            start: 0,
                            end: 50
                        }
                    }}
                    duration={750}
                >
                    <circle radius={0} top={84} left={72} />
                </animation>

                <animation
                    animationProps={{
                        top: {
                            start: -30,
                            end: 10
                        }
                    }}
                    duration={750}
                >
                    <text
                        height={30}
                        top={-30}
                        width={144}
                    >
                        {what}
                    </text>
                </animation>

                {'Rocks'.split('').map((letter, index) => (
                    <animation
                        animationProps={{
                            left: {
                                start: index % 2 == 0 ? letterWidth * -1 : 200,
                                end: (index * letterWidth)
                            }
                        }}
                        duration={2000}
                    >
                        <text
                            height={30}
                            left={index % 2 == 0 ? letterWidth * -1 : 200}
                            top={65}
                            width={letterWidth}
                        >
                            {letter}
                        </text>
                    </animation>
                ))}
            </Fragment>
        );
    }
}

export default App;