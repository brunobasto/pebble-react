import 'babel-polyfill'

import App from './TextMemoryUsageTestApp';
import React from 'react';
import render from "./pebble/render";

Pebble.addEventListener('ready', () => {
    (async () => {
        await render(<App />);
    })();
});