#include "snake/board.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace SnakeEngine;

int main() {
  Board game_board(24, 21);
  Move current_move = Move::kUp;
  while (!game_board.IsGameOver()) {
    std::cout << "\033[2J\033[H";
    game_board.DisplayBoard();
    std::cout.flush();
    auto result = game_board.ApplyMove(current_move);
    if (!result.ok()) {
      std::cout << "Game Over! Score: " << game_board.GetScore() << '\n';
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}