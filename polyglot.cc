#include <sstream>
#include <napi.h>
#include "utils.h"


/** Calculates the polyglot hash of a position
*   param pieceOffsets Offsets of pieces in the Random64 array
*   param castleOffsets Offsets of castling availability in the Random64 array
*   param enPassantOffset Offset of the en passant square in the Random64 array
*   param turnOffset Offset of the turn in the Random64 array
*/
Napi::Value Hash(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 4) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsArray()) {
    Napi::TypeError::New(env, "First argument should be an array")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[1].IsArray()) {
    Napi::TypeError::New(env, "Second argument should be an array")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[2].IsNumber()) {
    Napi::TypeError::New(env, "Third argument should be a number")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[3].IsBoolean()) {
    Napi::TypeError::New(env, "Fourth argument should be a boolean")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Array pieceOffsets = info[0].As<Napi::Array>();
  Napi::Array castlingOffsets = info[1].As<Napi::Array>();
  int32_t epOffset = info[2].ToNumber().Int32Value();
  bool isWhitesTurn = info[3].ToBoolean().Value();

  int32_t offset = 0;
  uint64_t hash = 0;
  for (uint32_t i = 0; i < pieceOffsets.Length(); ++i) {
    offset = pieceOffsets.Get(i).ToNumber().Int32Value();
    hash ^= RandomPiece[offset];
  }
  for (uint32_t i = 0; i < castlingOffsets.Length(); ++i) {
    offset = castlingOffsets.Get(i).ToNumber().Int32Value();
    hash ^= RandomCastle[offset];
  }
  if (epOffset >= 0) {
    hash ^= RandomEnPassant[epOffset];
  }
  if (isWhitesTurn) {
    hash ^= RandomTurn[0];
  }
  std::ostringstream o;
  o << hash;
  return Napi::String::New(env, o.str());
}

/** Finds the move corresponding to the hash in a book file
*   param hash A polyglot hash of the position
*   param bookFile The book file path
*   param findBest A flag specifying whether to find the best move or a random one
*/
Napi::Value Find(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "First argument should be a string")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[1].IsString()) {
    Napi::TypeError::New(env, "Second argument should be a string")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[2].IsBoolean()) {
    Napi::TypeError::New(env, "Third argument should be a boolean")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string temp = info[0].ToString().Utf8Value();
  std::istringstream i(temp);
  uint64_t hash64;
  i >> hash64;
  std::string bookFile = info[1].ToString().Utf8Value();
  bool searchBest = info[2].ToBoolean().Value();

  uint16_t move = FindInternal(hash64, bookFile.c_str(), searchBest);
  char moveStr[6];
  MoveToString(moveStr, move);
  return Napi::String::New(env, std::string(moveStr));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hash"), Napi::Function::New(env, Hash));
  exports.Set(Napi::String::New(env, "find"), Napi::Function::New(env, Find));
  return exports;
}

NODE_API_MODULE(addon, Init)
