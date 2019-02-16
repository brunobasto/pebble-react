import React, { Component } from 'react';

class App extends Component {
    state = {
        text: 'start text',
        textHeight: 80,
        textTop: 0,
        textWidth: 40
    };

    componentDidMount() {
        setTimeout(() => this.setState({
            text: 'new text',
            textHeight: 120,
            textWidth: 40,
            textTop: 30
        }), 1000);
    }

    render() {
        const { text, textHeight, textTop, textWidth } = this.state;

        return (
            <window name={text}>
                <text
                    height={textHeight}
                    left={0}
                    top={textTop}
                    width={textWidth}
                    key={'a'}
                >
                    {'a'}
                </text>

                {textTop === 0 && (
                    <text
                        height={textHeight}
                        left={50}
                        top={textTop + 100}
                        width={textWidth}
                        key={'b'}
                    >
                        {'b'}
                    </text>
                )}

                <text
                    height={textHeight}
                    left={100}
                    key={'c'}
                    top={textTop + 50}
                    width={textWidth}
                >
                    {'c'}
                </text>
            </window>
        );
    }
}

export default App;