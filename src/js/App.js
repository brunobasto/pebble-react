import React, { Component } from 'react';

class App extends Component {
    state = {what: 'React'};

    componentDidMount() {
        Pebble.addEventListener("appmessage", (e) => {
            this.setState({what: 'Pebble'});
        });
    }

    render() {
        const {what} = this.state;

        return (
            <window name="some">
                {what.split('').map((letter, index) => (
                    <animation
                        animationProps={{
                            top: {
                                start: (index * -30) - 30,
                                end: 50
                            }
                        }}
                        duration={750}
                    >
                        <text
                            height={30}
                            left={index * 30}
                            top={-1000}
                            width={25}
                        >
                            {letter}
                        </text>
                    </animation>
                ))}
                {'Rocks'.split('').map((letter, index) => (
                    <animation
                        animationProps={{
                            left: {
                                start: 1000,
                                end: (index * 30)
                            }
                        }}
                        duration={1500}
                    >
                        <text
                            height={30}
                            left={-1000}
                            top={80}
                            width={25}
                        >
                            {letter}
                        </text>
                    </animation>
                ))}
            </window>
        );
    }
}

export default App;