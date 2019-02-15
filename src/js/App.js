import React, { Component } from 'react';

class App extends Component {
    state = { text: 'start text', textHeight: 60, textTop: 0 };

    componentDidMount() {
        setTimeout(() => this.setState({
            text: 'new text',
            textHeight: 120,
            textTop: 30
        }), 1000);
    }

    render() {
        const { text, textHeight, textTop } = this.state;

        return (
            <window name={text}>
                <text
                    height={textHeight}
                    left={0}
                    top={textTop}
                    width={20}
                >
                    {'a'}
                </text>

                {textTop === 0 && (
                    <text
                        height={textHeight}
                        left={45}
                        top={textTop}
                        width={20}
                    >
                        {text}
                    </text>
                )}

                <text
                    height={textHeight}
                    left={90}
                    top={textTop}
                    width={20}
                >
                    {'c'}
                </text>
            </window>
        );
    }
}

export default App;