import React, { Component, Fragment } from 'react';

class App extends Component {
    state = { show: true, what: 'React' };

    componentDidMount() {
        // setTimeout(
        //     () => {
        //         this.setState({ show: false });
        //     },
        //     2000
        // );
        Pebble.addEventListener(
            'appmessage',
            (e) => {
                this.setState(
                    {
                        what: 'Peble'
                    }
                );
            }
        );
    }

    render() {
        const { show, what } = this.state;

        return (
            show && (
                <Fragment>
                    {what.split('').map((letter, index) => (
                        <text
                            alignment={'center'}
                            height={30}
                            left={index * 29}
                            top={50}
                            width={29}
                        >
                            {letter}
                        </text>
                ))}
                </Fragment>
            )
        );
    }
}

export default App;