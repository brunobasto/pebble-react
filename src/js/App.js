import React, { Component } from 'react';

class App extends Component {
    render() {
        return (
            <window name="some">
                {'React'.split('').map((letter, index) => (
                    <animation
                        animationProps={{
                            top: {
                                start: (index * -30) - 30,
                                end: 50
                            }
                        }}
                        duration={2000}
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
                        duration={3000}
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