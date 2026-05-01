#include "snake/board.h"

#include <deque>
#include <random>
#include <vector>

namespace SnakeEngine {
struct Board::Impl {
  struct Position {
    int row;
    int col;
  };

  std::vector<Cell> grid;
  std::deque<Position> snake;
  Move snake_direction;
  int width;
  int height;
  int score;
  bool game_over;
  std::mt19937 rng{std::random_device{}()};

  // helper to convert 2D to 1D index for flatmap
  int Index(int row, int col) const { return row * width + col; }

  // helper to check if the snake is out of bounds
  bool IsOutOfBounds() const {
    return snake.front().row >= height || snake.front().row < 0 ||
           snake.front().col >= width || snake.front().col < 0;
  }

  // helper to check if the snake is colliding with its own body
  bool IsCollidingWithBody() const {
    Position head = snake.front();
    for (size_t i = 1; i < snake.size(); ++i) {
      const Position &body = snake[i];
      if (body.row == head.row && body.col == head.col) {
        return true;
      }
    }
    return false;
  }

  // Initialize the grid
  void InitializeGrid() {
    grid.assign(width * height, Cell::kEmpty);
    score = 0;
    game_over = false;
  }

  // Spawn the snake at the middle of the board
  void PlaceSnake() {
    int row = height / 2;
    int col = width / 2;
    int snake_index = Index(row, col);
    grid[snake_index] = Cell::kSnakeHead;
    snake_direction = Move::kUp;
    snake.push_back({row, col});
  }

  // Randomly spawn an apple on an empty cell
  void SpawnApple() {
    std::vector<int> empty_cells;
    for (int i = 0; i < height * width; ++i) {
      if (grid[i] == Cell::kEmpty) {
        empty_cells.push_back(i);
      }
    }

    std::uniform_int_distribution<int> dist(0, empty_cells.size() - 1);
    int chosen = dist(rng);
    grid[chosen] = Cell::kApple;
  }

  // Determines if the move given is valid given the snake's current direction
  bool IsValidMove(Move move) {
    switch (snake_direction) {
    case Move::kUp:
      return move != Move::kDown;
    case Move::kDown:
      return move != Move::kUp;
    case Move::kRight:
      return move != Move::kLeft;
    case Move::kLeft:
      return move != Move::kRight;
    default:
      return false;
    }
  }

  const char *GetCellCharacter(int i) {
    switch (grid[i]) {
    case Cell::kEmpty:
      return ".";
    case Cell::kSnakeHead:
      return "@";
    case Cell::kSnakeBody:
      return "o";
    case Cell::kApple:
      return "*";
    default:
      return "";
    }
  }
};

Board::Board(int width, int height) : impl_(std::make_unique<Impl>()) {
  impl_->width = width;
  impl_->height = height;

  impl_->InitializeGrid();
  impl_->PlaceSnake();
  impl_->SpawnApple();
}

Board::~Board() = default;

bool Board::IsGameOver() const {
  return impl_->IsOutOfBounds() || impl_->IsCollidingWithBody();
}

absl::StatusOr<int> Board::ApplyMove(Move move) {
  // Return error status for invalid move given
  if (!impl_->IsValidMove(move)) {
    return absl::InvalidArgumentError("Move given is invalid");
  }
  return impl_->score;
}

void Board::DisplayBoard() const {
  for (int i = 0; i < impl_->width * impl_->height; ++i) {
    std::cout << impl_->GetCellCharacter(i);
    if ((i + 1) % impl_->width == 0) {
      std::cout << '\n';
    }
  }
}

} // namespace SnakeEngine