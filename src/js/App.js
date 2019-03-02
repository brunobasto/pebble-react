import React, { Component, Fragment } from 'react';

const letterWidth = 29;

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
            <Fragment>
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
                        alignment="center"
                        height={30}
                        top={-30}
                        width={130}
                    >
                        {what}
                    </text>
                </animation>
                {'Rocks'.split('').map((letter, index) => (
                    <animation
                        animationProps={{
                            left: {
                                start: letterWidth * -1,
                                end: (index * letterWidth)
                            }
                        }}
                        duration={750}
                        key={`${index}anim`}
                    >
                        <text
                            alignment="center"
                            height={30}
                            left={letterWidth * -1}
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