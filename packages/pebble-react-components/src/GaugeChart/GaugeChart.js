import React, { Component, Fragment } from 'react';
import {perc2color} from './utils';

class GaugeChart extends Component {
    static defaultProps = {
        emptyColor: '#81898c',
        left: 0,
        maxValue: 100,
        minValue: 0,
        padding: 3,
        sections: 5,
        thickness: 10,
        top: 0,
        value: 0,
        width: 144
    };

    getSectionColor(index) {
        const { sections } = this.props;
        const ratio = 1 - (index / (sections - 1));

        return perc2color(ratio * 100, 0, 100);
    }

    getSectionSize() {
        const { padding, sections } = this.props;

        return (180 - padding * (sections - 1)) / sections;
    }

    renderSections() {
        const {
            emptyColor,
            left,
            maxValue,
            minValue,
            padding,
            thickness,
            top,
            value,
            width
        } = this.props;
        const arcs = [];
        const sectionSize = this.getSectionSize();

        const angleValue = (value / (maxValue - minValue)) * 180 - 90;

        const arcProps = {
            left: left + width / 2,
            radius: width / 2,
            thickness: thickness,
            top: top + width / 2
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
        return (
            <Fragment>
                {this.renderSections()}
            </Fragment>
        );
    }
}

export default GaugeChart;