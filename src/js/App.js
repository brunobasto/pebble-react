import React, { Component, Fragment } from 'react';
const imageSize = 76;
const pebbleWidth = 140;
const pebbleHeight = 168;

class App extends Component {
    state = {
        loading: false,
        text: 'start text',
        textHeight: 80,
        textTop: 0,
        textWidth: 40
    };

    componentDidMount() {
        setTimeout(
            () => this.setState({
                loading: true,
                text: 'new text',
                textHeight: 120,
                textWidth: 40,
                textTop: 30
            }
            ), 3000);
    }

    render() {
        const { loading, text, textHeight, textTop, textWidth } = this.state;

        return (
            <window name={text}>
                {loading === false && (
                    <Fragment>
                        <text
                            height={textHeight}
                            left={0}
                            top={textTop}
                            width={textWidth}
                        >
                            {'a'}
                        </text>
                        <text
                            height={textHeight}
                            left={50}
                            top={textTop + 50}
                            width={textWidth}
                        >
                            {'b'}
                        </text>
                        <text
                            height={textHeight}
                            left={100}
                            top={textTop + 100}
                            width={textWidth}
                        >
                            {'c'}
                        </text>
                    </Fragment>
                )}
                {loading === true && (
                    <image
                        height={imageSize}
                        left={pebbleWidth/2 - imageSize/2}
                        top={pebbleHeight/2 - imageSize/2}
                        width={imageSize}
                    />
                )}
            </window>
        );
    }
}

export default App;