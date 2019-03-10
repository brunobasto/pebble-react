import React, { Component, Fragment } from 'react';

class HappyFace extends Component {
    render() {
        return (
            <Fragment>
                <circle top={25} left={40} radius={5} />

                <animation
                    animationProps={{
                        radius: {
                            start: 0,
                            end: 5
                        }
                    }}
                    delay={1750}
                    duration={250}
                >
                    <circle top={25} left={100} radius={5} />
                </animation>

                <arc
                    top={30}
                    left={70}
                    radius={50}
                    startAngle={90}
                    endAngle={270}
                />
            </Fragment>
        );
    }
}

export default HappyFace;