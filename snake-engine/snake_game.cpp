#include "snake/board.h"

#include <ncurses.h>

#include <chrono>
#include <thread>

using namespace SnakeEngine;

static const char* CellToChar(Cell cell) {
  switch (cell) {
  case Cell::kEmpty:     return ".";
  case Cell::kSnakeHead: return "@";
  case Cell::kSnakeBody: return "o";
  case Cell::kApple:     return "*";
  default:               return " ";
  }
}

int main() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  Board game_board(24, 21);
  Move current_move = Move::kUp;

  while (!game_board.IsGameOver()) {
    int ch = getch();
    switch (ch) {
    case KEY_UP:    current_move = Move::kUp;    break;
    case KEY_DOWN:  current_move = Move::kDown;  break;
    case KEY_LEFT:  current_move = Move::kLeft;  break;
    case KEY_RIGHT: current_move = Move::kRight; break;
    default: break;
    }

    auto result = game_board.ApplyMove(current_move);

    clear();
    const auto& grid = game_board.GetGrid();
    int width = game_board.GetWidth();
    int height = game_board.GetHeight();
    for (int i = 0; i < width * height; ++i) {
      addstr(CellToChar(grid[i]));
      if ((i + 1) % width == 0) addch('\n');
    }
    refresh();

    if (!result.ok() &&
        result.status().code() == absl::StatusCode::kOutOfRange) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  endwin();
  printf("Game Over! Score: %d\n", game_board.GetScore());

  return 0;
}
