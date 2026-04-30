#pragma once

#include <memory>

#include "absl/status/status.h"
#include "absl/status/statusor.h"

namespace SnakeEngine {
enum class Cell { kEmpty, kSnakeBody, kSnakeHead, kApple };

enum class Move { kUp, kDown, kLeft, kRight };
class Board {
public:
  Board(int width, int height);
  ~Board();

  Board(const Board &) = delete;
  Board &operator=(const Board &) = delete;

  // apply the move, return the current score after the move
  int applyMove(Move move);
  bool IsGameOver() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace SnakeEngine