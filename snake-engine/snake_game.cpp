#include "snake/board.h"
#include <iostream>

using namespace SnakeEngine;

int main() {
  Board game_board(21, 24);
  while (!game_board.IsGameOver()) {
    game_board.DisplayBoard();
    std::cout << "\033[2J\033[H";
  }
  return 0;
}