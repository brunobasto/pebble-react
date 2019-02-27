import React, { Component, Fragment } from 'react';

const letterWidth = 29;

class App extends Component {
    state = {what: 'React'};

    componentDidMount() {
        Pebble.addEventListener("appmessage", (e) => {
            // this.setState({what: 'Pebble'});
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
                            end: 140
                        }
                    }}
                    duration={1500}
                >
                    <text
                        alignment="center"
                        height={30}
                        width={130}
                    >
                        {'React'}
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
                        duration={1500}
                        key={`${index}anim`}
                    >
                        <text
                            alignment="center"
                            height={30}
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