import 'babel-polyfill'

import App from './App';
import React from 'react';
import render from "./pebble/render";

Pebble.addEventListener('ready', () => {
    console.log('Pebble ready');
    (async () => {
        await render(<App />);
    })();
});

