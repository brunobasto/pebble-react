const path = require('path');
const webpack = require('webpack');

function resolveModule(name) {
  return path.resolve(__dirname, `src/${name}`);
}

const config = {
  entry: [
    resolveModule('index.js')
  ],
  mode: 'development',
  module: {
    rules: [
      {
        test: /\.(svg|png|jpe?g|gif)$/i,
        use: [
          {
            loader: 'file-loader',
            options: {
              name: '[path][name].[ext]',
            },
          },
        ],
      },
      {
        test: /\.css$/i,
        use: ['style-loader', 'css-loader'],
      },
      {
        test: /\.js$/,
        use: {
          loader: 'babel-loader',
          options: {
            exclude: ['node_modules/@babel'],
            include: ['./src', '../pebble-react-core'],
            presets: [
              ["@babel/preset-env", {
                useBuiltIns: 'usage',
                targets: {
                  "browsers": ["last 2 versions", "ie >= 7"]
                },
                "modules": false
              }],
              "@babel/preset-react"
            ],
            plugins: [
              "@babel/plugin-transform-async-to-generator"
            ]
          }
        }
      }
    ]
  },
  output: {
    filename: 'bundle.js',
    path: path.resolve('build/src/js')
  },
  plugins: [
    new webpack.EnvironmentPlugin(['NODE_ENV', 'CLIENT_URL'])
  ]
};

module.exports = config;