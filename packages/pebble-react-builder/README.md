This tool should be used to build an app create with pebble-react-create

It should be able to copy the C files from pebble-react-core inside the node_modules folder of the boilerplate app to a build folder.

Then it should copy the messageKeys and app displayName into the package.json file.

It should have a command to proxy to the pebble command line so that we can call "pebble build" or "pebble install" inside the correct folder.