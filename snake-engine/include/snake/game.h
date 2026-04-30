#pragma once

#include <memory>

#include "absl/status/status.h"
#include "snake/board.h"

namespace SnakeEngine {
class SnakeGame {
public:
  SnakeGame(int width, int height);
  // Destructor
  ~SnakeGame();

  // Disallow Copying
  SnakeGame(const SnakeGame &) = delete;
  SnakeGame &operator=(const SnakeGame &) = delete;

  // Runs the game
  absl::Status startGame();

  bool IsGameOver() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace SnakeEngine