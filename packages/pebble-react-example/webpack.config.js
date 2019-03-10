const CleanWebpackPlugin = require('clean-webpack-plugin');
const path = require('path');
const webpack = require('webpack');

function resolveModule(name) {
  return path.resolve(__dirname, `src/${name}`);
}

const config = {
  entry: [
    resolveModule('index.js')
  ],
  module: {
    loaders: [
      {
        exclude: [
          /node_modules/
        ],
        // include: __dirname,
        loaders: ['babel-loader'],
      },
      {
        loaders: ['json-loader'],
        test: /\.json$/
      }
    ]
  },
  output: {
    filename: 'bundle.js',
    path: path.resolve('build/react/src/js')
  },
  plugins: [
    new webpack.DefinePlugin({ "process.env.NODE_ENV": JSON.stringify("production") }),
    new CleanWebpackPlugin(),
    new webpack.optimize.UglifyJsPlugin({sourceMap: true})
  ]
};

module.exports = config;