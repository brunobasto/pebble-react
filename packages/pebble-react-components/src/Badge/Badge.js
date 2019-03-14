import React, { Component, Fragment } from 'react';

class Badge extends Component {
    static defaultProps = {
		borderRadius: 5,
        color: '#0AB503',
		left: 0,
		height: 25,
        padding: 4,
        top: 0,
        value: 0,
        width: 124
    };

    render() {
        const {
			borderRadius,
			children,
			color,
			left,
			top,
			width,
			height
		} = this.props;

		const arcProps = {
			color,
			thickness: borderRadius,
			radius: borderRadius
        };

        return (
            <Fragment>
                <arc
					{...arcProps}
					startAngle={-90}
					endAngle={0}
					center={{x: left + borderRadius, y: top + borderRadius}}
				/>
				<arc
					{...arcProps}
					startAngle={180}
					endAngle={270}
					center={{x: left + borderRadius, y: top + height - borderRadius}}
				/>
				<arc
					{...arcProps}
					startAngle={90}
					endAngle={180}
					center={{x: left + width - borderRadius, y: top + height - borderRadius}}
				/>
				<arc
					{...arcProps}
					startAngle={0}
					endAngle={90}
					center={{x: left + width - borderRadius, y: top + borderRadius}}
				/>
				<path
					fillColor={color}
					points={[
						{ x: left + borderRadius, y: top },
						{ x: left, y: top + borderRadius },
						{ x: left, y: top + height - borderRadius },
						{ x: left + borderRadius, y: top + height },
						{ x: left + width - borderRadius, y: top + height },
						{ x: left + width, y: top + height - borderRadius },
						{ x: left + width, y: top + borderRadius },
						{ x: left + width - borderRadius, y: top }
					]}
					strokeColor={color}
				/>
				{children}
            </Fragment>
        );
    }
}

export default Badge;