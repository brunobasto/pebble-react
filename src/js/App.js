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
        setTimeout(
            () => {
                this.setState({loading: true});
                setTimeout(() => this.setState({loading: false, text: 'Rocks'}), 3000);
            },
            3000
        );
    }

    render() {
        const { loading, text } = this.state;

        return (
            <window>
                {loading === false && (
                    <Fragment>
                        {text.split('').map((letter, index) => (
                            <text
                                height={textSize + 5}
                                left={index * 30}
                                top={index * 30}
                                width={textSize}
                            >
                                {letter}
                            </text>
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