#!/usr/bin/env node

const { ncp } = require('ncp');
const fs = require('fs');
const mkdirp = require('mkdirp');
const path = require('path');
const rimraf = require("rimraf");
var download = require('github-download-directory');
const { exec } = require('child_process');
const commandExists = require('command-exists');

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
	.command(
		'create [name]',
		'Creates a boilerplate application.',
		(yargs) => {
			yargs
				.positional(
					'name', {
						describe: 'The name of your application',
						default: 'Example'
					}
				)
		},
		(argv) => {
			const targetFolder = path.join(process.cwd(), argv.name);

			if (!fs.existsSync(targetFolder)) {
				mkdirp.sync(targetFolder);
			}
			else {
				console.log(`The folder ${targetFolder} already exists. Please try this on an empty directory.`);

				process.exit(1);
			}

			console.log('Downloading boilerplate...');

			download('brunobasto', 'pebble-react', 'packages', { sha: '2fa4260874e252b84a4bbcc5fc037f81e066ecee' })
				.then(() => {
					ncp(
						path.join(process.cwd(), 'packages', 'pebble-react-example'),
						targetFolder,
						(error) => {
							if (error) {
								console.log('Error creating boilerplate', error);

								return;
							}

							rimraf.sync(path.join(process.cwd(), 'packages'));

							console.log(`
Successfully created boilerplate. Your application structure is:

Directory: ${targetFolder}

- package.json
- webpack.config.js
- src/
  - App.js    [Modify this file and run "npm run build" and then "npm run emulator" to experiment with it]
  - index.js  [The entry point containing the render() call to initialize your App]

We already have configured your build scripts for you with support for ES6 syntax and webpack.
							`);

							commandExists('pebble', (err, commandExists) => {
								const configurations = ['npm install', 'npm run build'];

								if (commandExists) {
									configurations.push('npm run emulator');

									console.log(`Cool! It looks like you already have the Pebble SDK installed.

We are now building your app for the first time and will try to start the emulator for you.
Please wait...
`);
								}
								else {
									console.log(`
It seems you don't have the Pebble SDK installed. Please install it so that you can compile your app and expriment with it using the emulator.
For now, we are just gonna run "npm install" for you and we are done.
`);
								}

								exec(configurations.join(' && '), {
									cwd: targetFolder
								}, function (error, stdout, stderr) {
									if (error) {
										console.log('Sorry, there was a problem building your app. Please checkout the error log:');
										console.log(stderr);
									}
									console.log('We are done! Your app should be running on the Pebble emulator now.');
								});
							});
						});
				});

		}
	)
	.argv