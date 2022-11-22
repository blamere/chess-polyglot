const assert = require('assert');
const Polyglot = require('../lib/polyglot.js');
const poly = new Polyglot();

const test_fen = (fen) => {
  const key = BigInt(poly.hash(fen));
  console.log("FEN=" + fen);
  console.log("key=" + key.toString(16));
  console.log("num=" + key);
};

const assert_key_equal = (fen, key) => {
  const expected_key = BigInt(key);
  const actual_key = BigInt(poly.hash(fen));
  assert(expected_key === actual_key);
};

assert_key_equal("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "0x463b96181691fc9c");
assert_key_equal("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", "0x823c9b50fd114196");
assert_key_equal("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2", "0x0756b94461c50fb0");
assert_key_equal("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2", "0x662fafb965db29d4");
assert_key_equal("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3", "0x22a48b5a8e47ff78");
assert_key_equal("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 3", "0x652a607ca3f242c1");
assert_key_equal("rnbq1bnr/ppp1pkpp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR w - - 0 4", "0x00fdd303c946bdd9");
assert_key_equal("rnbqkbnr/p1pppppp/8/8/PpP4P/8/1P1PPPP1/RNBQKBNR b KQkq c3 0 3", "0x3c8123ea7b067637");
assert_key_equal("rnbqkbnr/p1pppppp/8/8/P6P/R1p5/1P1PPPP1/1NBQKBNR b Kkq - 0 4", "0x5c3f9b829b279560");
