import React, { Component, Fragment } from 'react';

const imageSize = 78;
const textSize = 25;
const pebbleWidth = 134;
const pebbleHeight = 156;

class App extends Component {
    state = {
        loading: false,
        text: 'React'
    };

    componentDidMount() {
        // setTimeout(
        //     () => {
        //         this.setState({ loading: true });
        //         setTimeout(() => this.setState({ loading: false, text: 'Rocks' }), 3000);
        //     },
        //     3000
        // );
    }

    render() {
        const { loading, text } = this.state;

        return (
            <window name="some">
                {loading === false && (
                    <Fragment>
                        {'React'.split('').map((letter, index) => (
                            <animation
                                animationProps={{
                                    top: {
                                        start: (index * -30) - 30,
                                        end: 50
                                    }
                                }}
                                duration={1000}
                            >
                                <text
                                    height={textSize + 5}
                                    left={index * 30}
                                    top={-1000}
                                    width={textSize}
                                >
                                    {letter}
                                </text>
                            </animation>
                        ))}
                        {'Rocks'.split('').map((letter, index) => (
                            <animation
                                animationProps={{
                                    left: {
                                        start: 500,
                                        end: (index * 30)
                                    }
                                }}
                                duration={2000}
                            >
                                <text
                                    height={textSize + 5}
                                    left={-1000}
                                    top={80}
                                    width={textSize}
                                >
                                    {letter}
                                </text>
                            </animation>
                        ))}
                    </Fragment>
                )}
                {loading === true && (
                    <image
                        height={imageSize}
                        left={pebbleWidth / 2 - imageSize / 2}
                        top={pebbleHeight / 2 - imageSize / 2}
                        width={imageSize}
                    />
                )}
            </window>
        );
    }
}

export default App;