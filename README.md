chess-polyglot
==============
chess-polyglot is a node module to grab moves from Polyglot-formatted
opening-book bin files.
[Polyglot book format](http://hgm.nubati.net/book_format.html)

A lot of this code has been copied from the [polyglot-chess](https://github.com/imor/polyglot/issues)
node module then updated to work with current versions of Node.js via
the [node-addon-api](https://github.com/nodejs/node-addon-api).

## Installation
    $ npm install chess-polyglot

## Example
```js
const Polyglot = require('chess-polyglot')

const poly = new Polyglot()

const fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
console.log("Polyglot hash for fen " + fen + " is: " + poly.hash(fen))
console.log("Best move in the book is: " + poly.find(fen, "book.bin", true))
console.log("Random move in the book is: " + poly.find(fen, "book.bin", false))
```

## API

### Functions
chess-polyglot exposes following functions -

#### find(fen, bookFile, findBest)
find takes a fen string and a book file path and looks up the hash in the book
file. If findBest is true it returns the best move according to the weight
otherwise it returns a random move with the given hash. The chance of a random
move being returned is in proportion to its weight. If no move is found it
returns an empty string.

#### hash(fen)
hash takes a fen string and returns its polyglot hash.

## Credits
Adapted from [polyglot-chess](https://github.com/imor/polyglot),
which is derivative of the excellent [stockfish](https://github.com/mcostalba/Stockfish)
polyglot book source code.

## License
chess-polyglot is released under the MIT License. See the bundled LICENSE file for
details.
