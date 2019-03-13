const CleanWebpackPlugin = require('clean-webpack-plugin');
const path = require('path');

function resolveModule(name) {
  return path.resolve(__dirname, `src/${name}`);
}

const config = {
  mode: 'production',
  target: 'web',
  entry: [
    resolveModule('index.js')
  ],
  module: {
    rules: [
      {
        test: /\.js$/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: [
              "@babel/preset-env",
              "@babel/preset-react"
            ],
            plugins: [
              [
                "@babel/plugin-proposal-class-properties"
              ]
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
    new CleanWebpackPlugin(),
  ]
};

module.exports = config;