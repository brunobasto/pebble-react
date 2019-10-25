import 'babel-polyfill'

import App from './App';
import React from 'react';
import render from "@pebble-react/core";

Pebble.addEventListener('ready', () => {
    render(<App />);
});