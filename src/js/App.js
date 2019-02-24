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
                <animation
                    animationProps={{
                        top: {
                            start: -30,
                            end: 40
                        }
                    }}
                    duration={1750}
                >
                    <text
                        alignment="center"
                        height={30}
                        left={20}
                        top={-30}
                        width={120}
                    >
                        {what}
                    </text>
                </animation>
                {'Rocks'.split('').map((letter, index) => (
                    <animation
                        animationProps={{
                            left: {
                                start: -25,
                                end: (index * 25)
                            }
                        }}
                        duration={1500}
                    >
                        <text
                            height={30}
                            left={-25}
                            top={120}
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