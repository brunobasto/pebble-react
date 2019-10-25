import 'core-js/features/set';
import 'core-js/features/map';
import 'core-js/features/promise'; 
import 'core-js/features/object'; 

import App from './App';
import React from 'react';
import render from "@pebble-react/core";

Pebble.addEventListener('ready', () => {
    render(<App />);
});