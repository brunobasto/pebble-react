import React, { useState, useEffect, Fragment } from 'react';
import HappyFace from './HappyFace';

const letterWidth = 29;

export default () => {
    const [what, setWhat] = useState('React');

    useEffect(() => {
        Pebble.addEventListener('appmessage', event => setWhat('Pebble'));
    }, [setWhat]);

    return (
        <Fragment>
            {/* Imported custom component */}
            <HappyFace />
            {/* Animated layer with nested animated circle */}
            <animation
                animationProps={{
                    left: {
                        start: -30,
                        end: 0
                    },
                    top: {
                        start: -30,
                        end: 90
                    }
                }}
                duration={750}
            >
                <layer
                    height={30}
                    top={-30}
                    width={144}
                >
                    <animation
                        animationProps={{
                            radius: {
                                start: 0,
                                end: 7
                            }
                        }}
                        delay={750}
                        duration={500}
                    >
                        <circle top={13} left={15} />
                    </animation>

                    <animation
                        animationProps={{
                            color: {
                                start: "#5F98E1",
                                end: "#073CAA"
                            }
                        }}
                        delay={750}
                        duration={500}
                    >
                        <text
                            alignment={'left'}
                            color={"#5F98E1"}
                            height={30}
                            left={30}
                            width={10 * what.length}
                        >
                            {what}
                        </text>
                    </animation>
                </layer>
            </animation>
            {/* Animated letters */}
            {'Rocks'.split('').map((letter, index) => (
                <animation
                    animationProps={{
                        color: {
                            start: "#5F98E1",
                            end: "#CCCB40"
                        },
                        left: {
                            start: index % 2 == 0 ? letterWidth * -1 : 200,
                            end: (index * letterWidth)
                        }
                    }}
                    duration={2000}
                    key={`${letter}${index}`}
                >
                    <text
                        color="#5F98E1"
                        height={30}
                        left={index % 2 == 0 ? letterWidth * -1 : 200}
                        top={120}
                        width={letterWidth}
                    >
                        {letter}
                    </text>
                </animation>
            ))}
        </Fragment>
    );
};