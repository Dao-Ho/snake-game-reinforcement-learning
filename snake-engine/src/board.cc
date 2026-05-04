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
  std::mt19937 rng{std::random_device{}()};

  // helper to convert 2D to 1D index for flatmap
  int Index(int row, int col) const { return row * width + col; }

  // helper to check if the snake is out of bounds
  bool IsOutOfBounds(int current_row, int current_col) const {
    return current_row >= height || current_row < 0 || current_col >= width ||
           current_col < 0;
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

  // Update the snake based on the move, if the new cell is an apple, spawn a
  // new apple and update the score
  void HandleAdvancingSnake(Move move) {
    Position new_head = DetermineUpdatedSnakeHeadPosition(move);
    if (IsOutOfBounds(new_head.row, new_head.col)) {
      snake.push_front(new_head);
      return;
    }
    Cell incoming = grid[Index(new_head.row, new_head.col)];
    HandleIncomingCellType(incoming, new_head);
  }

  void AdvanceSnake(Position updated_position, bool is_growing) {
    // Advance the head
    Position current_head = snake.front();
    grid[Index(current_head.row, current_head.col)] = Cell::kSnakeBody;
    grid[Index(updated_position.row, updated_position.col)] = Cell::kSnakeHead;
    snake.push_front(updated_position);

    if (!is_growing) {
      // Remove/advance the old tail
      Position current_tail = snake.back();
      grid[Index(current_tail.row, current_tail.col)] = Cell::kEmpty;
      snake.pop_back();
    }
  }

  void UpdateScore() { ++score; }

  void HandleIncomingCellType(Cell cell_type, Position updated_position) {
    switch (cell_type) {
    case Cell::kEmpty:
      AdvanceSnake(updated_position, false);
      break;
    case Cell::kApple:
      AdvanceSnake(updated_position, true);
      SpawnApple();
      UpdateScore();
      break;
    case Cell::kSnakeBody:
    case Cell::kSnakeHead:
      snake.push_front(updated_position);

      return;
    }
  }

  // Determine the updated position of the snake's head
  Position DetermineUpdatedSnakeHeadPosition(Move move) {
    switch (move) {
    case Move::kUp:
      return {snake.front().row - 1, snake.front().col};
    case Move::kDown:
      return {snake.front().row + 1, snake.front().col};
    case Move::kRight:
      return {snake.front().row, snake.front().col + 1};
    case Move::kLeft:
      return {snake.front().row, snake.front().col - 1};
    default:
      return {snake.front().row, snake.front().col};
    }
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
  return impl_->IsOutOfBounds(impl_->snake.front().row,
                              impl_->snake.front().col) ||
         impl_->IsCollidingWithBody();
}

int Board::GetScore() const { return impl_->score; }

absl::StatusOr<int> Board::ApplyMove(Move move) {
  // Fall back to current direction on invalid move (e.g. reverse direction)
  if (!impl_->IsValidMove(move)) {
    move = impl_->snake_direction;
  }

  // Advance the snake to the updated cell
  impl_->HandleAdvancingSnake(move);
  impl_->snake_direction = move;

  // Determine if game is over
  if (IsGameOver()) {
    return absl::OutOfRangeError("Game is over");
  }

  // Return the score
  return impl_->score;
}

int Board::GetWidth() const { return impl_->width; }

int Board::GetHeight() const { return impl_->height; }

const std::vector<Cell> &Board::GetGrid() const { return impl_->grid; }

} // namespace SnakeEngine