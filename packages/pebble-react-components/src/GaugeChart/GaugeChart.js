import React, { Component, Fragment } from 'react';
import { perc2color } from './utils';

class GaugeChart extends Component {
    static defaultProps = {
        emptyColor: '#81898c',
        left: 0,
        maxValue: 100,
        minValue: 0,
        padding: 3,
        sections: 5,
        showPointer: true,
        thickness: 10,
        top: 0,
        value: 0,
        width: 144
    };

    getCenter() {
        const {
            left,
            top,
            width
        } = this.props;

        return {
            x: left + width / 2,
            y: top + width / 2
        }
    }

    getSectionColor(index) {
        const { sections } = this.props;
        const ratio = 1 - (index / (sections - 1));

        return perc2color(ratio * 100, 0, 100);
    }

    getSectionSize() {
        const { padding, sections } = this.props;

        return (180 - padding * (sections - 1)) / sections;
    }

    getValueAngle(value) {
        const {
            maxValue,
            minValue
        } = this.props;

        const currentValue = Math.min(maxValue, Math.max(minValue, value));

        return (currentValue / (maxValue - minValue)) * 180;
    }

    renderPointer() {
        const { minValue, thickness, width, value } = this.props;
        const center = this.getCenter();
        const pointerHeight = (width / 2) - (thickness * 1.25);
        const pointerRadius = 6;
        const valueAngle = this.getValueAngle(value);
        const minValueAngle = this.getValueAngle(minValue);

        return (
            <Fragment>
                <circle
                    left={center.x}
                    radius={pointerRadius}
                    top={center.y}
                />
                <animation
                    animationProps={{
                        rotationAngle: {
                            start: minValueAngle * -1,
                            end: valueAngle * -1
                        }
                    }}
                    duration={1000}
                >
                    <path
                        points={[
                            { x: center.x, y: center.y - pointerHeight },
                            { x: center.x - pointerRadius, y: center.y },
                            { x: center.x + pointerRadius, y: center.y }
                        ]}
                        rotationPoint={center}
                        rotationAngle={minValueAngle * -1}
                    />
                </animation>
            </Fragment>
        );
    }

    renderSections() {
        const {
            emptyColor,
            padding,
            thickness,
            width,
            value
        } = this.props;
        const arcs = [];
        const sectionSize = this.getSectionSize();
        const valueAngle = this.getValueAngle(value);
        const arcProps = {
            center: this.getCenter(),
            radius: width / 2,
            thickness: thickness,
        };
        let index = 0;

        for (let startAngle = -90; startAngle < 90; startAngle += sectionSize) {
            let endAngle = startAngle + sectionSize;

            if (startAngle < valueAngle && endAngle > valueAngle) {
                endAngle = valueAngle;

                arcs.push(
                    <arc
                        {...arcProps}
                        color={emptyColor}
                        startAngle={valueAngle}
                        endAngle={startAngle + sectionSize}
                    />
                );
            }

            arcs.push(
                <arc
                    {...arcProps}
                    color={startAngle > valueAngle ? emptyColor : this.getSectionColor(index)}
                    startAngle={startAngle}
                    endAngle={endAngle}
                />
            );
            index++;
            startAngle += padding;
        }

        return arcs;
    }

    render() {
        const { showPointer } = this.props;

        return (
            <Fragment>
                {this.renderSections()}

                {showPointer && this.renderPointer()}
            </Fragment>
        );
    }
}

export default GaugeChart;