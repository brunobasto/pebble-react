import React, { Component, Fragment } from 'react';

class GaugeChart extends Component {
    render() {
		const {
			left,
			top,
			thickness,
			width,
			height
		} = this.props;

        return (
            <Fragment>
                <arc
                    top={30}
                    left={70}
                    radius={50}
                    startAngle={-90}
                    thickness={thickness}
                    endAngle={-30}
                />

				<arc
                    top={30}
                    left={70}
                    radius={50}
                    startAngle={-20}
                    thickness={thickness}
                    endAngle={40}
                />

                <arc
                    top={30}
                    left={70}
                    radius={50}
                    startAngle={50}
                    thickness={thickness}
                    endAngle={90}
                />
            </Fragment>
        );
    }
}

export default GaugeChart;