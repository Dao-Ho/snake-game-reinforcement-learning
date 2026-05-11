import argparse
import curses
import time

from env_wrapper import (
    action_idx_to_move,
    grid_to_state,
    snake_engine,
)
import checkpoint
from agent import SnakeGameAgent

WIDTH = 24
HEIGHT = 21
FRAME_DELAY = 0.1


_CELL_CHARS = {
    snake_engine.Cell.kEmpty: ". ",
    snake_engine.Cell.kSnakeHead: "@ ",
    snake_engine.Cell.kSnakeBody: "o ",
    snake_engine.Cell.kApple: "* ",
}


def render(stdscr, board) -> None:
    stdscr.clear()
    grid = board.get_grid()
    width = board.get_width()
    height = board.get_height()
    for r in range(height):
        row_chars = "".join(
            _CELL_CHARS[grid[r * width + c]] for c in range(width)
        )
        stdscr.addstr(r, 0, row_chars)
    stdscr.addstr(height, 0, f"Score: {board.get_score()}")
    stdscr.refresh()


def play(stdscr, checkpoint_name: str) -> int:
    curses.curs_set(0)
    stdscr.nodelay(True)

    agent = SnakeGameAgent()
    checkpoint.load(agent, checkpoint_name)
    agent.epsilon = 0.0  # pure greedy

    board = snake_engine.Board(WIDTH, HEIGHT)

    while not board.is_game_over():
        state = grid_to_state(board)
        action_idx = agent.select_action(state)
        move = action_idx_to_move(action_idx)

        try:
            board.apply_move(move)
        except ValueError:
            pass

        render(stdscr, board)
        time.sleep(FRAME_DELAY)

    render(stdscr, board)
    stdscr.addstr(HEIGHT + 2, 0, "Game Over! Press any key to exit.")
    stdscr.nodelay(False)
    stdscr.getch()
    return board.get_score()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--checkpoint", default="best",
                        help="Checkpoint name to load (without .pth)")
    args = parser.parse_args()

    final_score = curses.wrapper(play, args.checkpoint)
    print(f"Final score: {final_score}")


if __name__ == "__main__":
    main()
