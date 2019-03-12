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
            left: left + width / 2,
            top: top + width / 2
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

    renderPointer() {
        const { thickness, width } = this.props;
        const center = this.getCenter();
        const pointerHeight = (width / 2) - (thickness * 1.25);
        const pointerRadius = 6;

        return (
            <Fragment>
                <circle left={center.left} top={center.top} radius={pointerRadius} />
                <path
                    points={[
                        { x: center.left, y: center.top - pointerHeight },
                        { x: center.left - pointerRadius, y: center.top },
                        { x: center.left + pointerRadius, y: center.top }
                    ]}
                />
            </Fragment>
        );
    }

    renderSections() {
        const {
            emptyColor,
            maxValue,
            minValue,
            padding,
            thickness,
            value,
            width
        } = this.props;
        const arcs = [];
        const sectionSize = this.getSectionSize();

        const angleValue = (value / (maxValue - minValue)) * 180 - 90;

        const arcProps = {
            ...this.getCenter(),
            radius: width / 2,
            thickness: thickness,
        };
        let index = 0;

        for (let startAngle = -90; startAngle < 90; startAngle += sectionSize) {
            let endAngle = startAngle + sectionSize;

            if (startAngle < angleValue && endAngle > angleValue) {
                endAngle = angleValue;

                arcs.push(
                    <arc
                        {...arcProps}
                        color={emptyColor}
                        startAngle={angleValue}
                        endAngle={startAngle + sectionSize}
                    />
                );
            }

            arcs.push(
                <arc
                    {...arcProps}
                    color={startAngle > angleValue ? emptyColor : this.getSectionColor(index)}
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
        const {showPointer} = this.props;

        return (
            <Fragment>
                {this.renderSections()}

                {showPointer && this.renderPointer()}
            </Fragment>
        );
    }
}

export default GaugeChart;