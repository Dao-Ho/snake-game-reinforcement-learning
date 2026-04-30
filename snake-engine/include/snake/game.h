#include <memory>

namespace SnakeEngine {
enum class Cell { kEmpty, kSnakeBody, kSnakeHead, kApple };

enum class Move { kUp, kDown, kLeft, kRight };

class SnakeGame {
public:
  SnakeGame();
  // Destructor
  ~SnakeGame();

  // Disallow Copying
  SnakeGame(const SnakeGame &) = delete;
  SnakeGame &operator=(const SnakeGame &) = delete;

  absl::Status makeMove(Move move);

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace SnakeEngine