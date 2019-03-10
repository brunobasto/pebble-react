#!/usr/bin/env node

const path = require('path');
const {ncp} = require('ncp');
const fs = require('fs');
const mkdirp = require('mkdirp');

// Current dir package.json
const cwdPackagePath = path.join(process.cwd(), 'package.json');

if (!fs.existsSync(cwdPackagePath)) {
	console.error('Please run pebble-react from a directory containing a package.json file.');
	process.exit(1);
}

const cwdPackage = require(cwdPackagePath);

// Path to @pebble-react/core
const corePackagePath = require.resolve("@pebble-react/core/package.json");
const corePath = path.dirname(corePackagePath);
const corePackage = require(corePackagePath);

require('yargs') // eslint-disable-line
	.command(
		'build [dest]',
		'Builds the application.',
		(yargs) => {
			const defaultDest = path.join(process.cwd(), "build");

			yargs
				.positional(
					'dest', {
						describe: 'Destination of build files.',
						default: defaultDest
					}
				)
		},
		(argv) => {
			// Copy C source files
			const cSourcePath = path.join(corePath, 'src', 'c');
			const cDestPath = path.join(argv.dest, 'src', 'c');

			if (!fs.existsSync(cDestPath)) {
				mkdirp.sync(cDestPath);
			}

			ncp(cSourcePath, cDestPath, (error) => {
				if (error) {
					console.log('Error copying C files', error);

					return;
				}

				console.log('Successfully copied C files.');
			});

			// Copy package.json
			const packageDestPath = path.join(argv.dest, 'package.json');

			const appPackageJSON = {
				author: cwdPackage.author,
				name: cwdPackage.name,
				pebble: corePackage.pebble,
				version: '0.0.0'
			};

			fs.writeFileSync(packageDestPath, JSON.stringify(appPackageJSON));

			// Copy wscript
			const wscriptSourcePath = path.join(corePath, 'wscript');
			const wscriptDestPath = path.join(argv.dest, 'wscript');

			ncp(wscriptSourcePath, wscriptDestPath, (error) => {
				if (error) {
					console.log('Error copying wscript', error);

					return;
				}

				console.log('Successfully copied wscript.');
			});
		}
	)
	.option(
		'verbose', {
			alias: 'v',
			default: false
		}
	)
	.argv