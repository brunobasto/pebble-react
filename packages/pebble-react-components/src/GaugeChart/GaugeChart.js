import React, { Component, Fragment } from 'react';
import {perc2color} from './utils';

class GaugeChart extends Component {
    static defaultProps = {
        padding: 3,
        sections: 3
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

    renderArcs() {
        const {
            padding,
            top,
            thickness
        } = this.props;
        const arcs = [];
        const sectionSize = this.getSectionSize();

        for (let startAngle = -90; startAngle < 90; startAngle += sectionSize) {
            arcs.push(
                <arc
                    color={this.getSectionColor(arcs.length)}
                    top={top}
                    left={70}
                    radius={50}
                    startAngle={startAngle}
                    thickness={thickness}
                    endAngle={startAngle + sectionSize}
                />
            );

            startAngle += padding;
        }

        return arcs;
    }

    render() {
        const {
            sections
        } = this.props;

        return (
            <Fragment>
                {this.renderArcs(sections)}
            </Fragment>
        );
    }
}

export default GaugeChart;