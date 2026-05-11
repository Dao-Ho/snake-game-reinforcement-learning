import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'snake-engine', 'build'))
import snake_engine_py as snake_engine

REWARD_APPLE = 10.0
REWARD_DEATH = -10.0
REWARD_STEP = -0.01

STATE_SIZE = 11

# (drow, dcol) deltas for each Move direction
_MOVE_DELTAS = {
    snake_engine.Move.kUp: (-1, 0),
    snake_engine.Move.kDown: (1, 0),
    snake_engine.Move.kLeft: (0, -1),
    snake_engine.Move.kRight: (0, 1),
}

# For each direction, what "left" and "right" relative turns mean.
_LEFT_OF = {
    snake_engine.Move.kUp: snake_engine.Move.kLeft,
    snake_engine.Move.kDown: snake_engine.Move.kRight,
    snake_engine.Move.kLeft: snake_engine.Move.kDown,
    snake_engine.Move.kRight: snake_engine.Move.kUp,
}
_RIGHT_OF = {
    snake_engine.Move.kUp: snake_engine.Move.kRight,
    snake_engine.Move.kDown: snake_engine.Move.kLeft,
    snake_engine.Move.kLeft: snake_engine.Move.kUp,
    snake_engine.Move.kRight: snake_engine.Move.kDown,
}


def action_idx_to_move(action_idx: int) -> snake_engine.Move:
    return [
        snake_engine.Move.kUp,
        snake_engine.Move.kDown,
        snake_engine.Move.kLeft,
        snake_engine.Move.kRight,
    ][action_idx]


def _find_positions(board):
    grid = board.get_grid()
    width = board.get_width()
    head = apple = None
    for i, cell in enumerate(grid):
        if cell == snake_engine.Cell.kSnakeHead:
            head = (i // width, i % width)
        elif cell == snake_engine.Cell.kApple:
            apple = (i // width, i % width)
    return head, apple


def _is_danger(board, row: int, col: int) -> bool:
    width = board.get_width()
    height = board.get_height()
    if row < 0 or row >= height or col < 0 or col >= width:
        return True
    cell = board.get_grid()[row * width + col]
    return cell == snake_engine.Cell.kSnakeBody


def grid_to_state(board) -> list[float]:
    head, apple = _find_positions(board)
    if head is None or apple is None:
        return [0.0] * STATE_SIZE

    direction = board.get_direction()
    hr, hc = head
    ar, ac = apple

    # Danger in three relative directions
    dr, dc = _MOVE_DELTAS[direction]
    danger_straight = _is_danger(board, hr + dr, hc + dc)

    ldr, ldc = _MOVE_DELTAS[_LEFT_OF[direction]]
    danger_left = _is_danger(board, hr + ldr, hc + ldc)

    rdr, rdc = _MOVE_DELTAS[_RIGHT_OF[direction]]
    danger_right = _is_danger(board, hr + rdr, hc + rdc)

    return [
        float(danger_straight),
        float(danger_left),
        float(danger_right),
        float(direction == snake_engine.Move.kUp),
        float(direction == snake_engine.Move.kDown),
        float(direction == snake_engine.Move.kLeft),
        float(direction == snake_engine.Move.kRight),
        float(ar < hr),  # apple is above
        float(ar > hr),  # apple is below
        float(ac < hc),  # apple is left of head
        float(ac > hc),  # apple is right of head
    ]


def compute_reward(score_before: int, score_after: int, done: bool) -> float:
    if done:
        return REWARD_DEATH
    if score_after > score_before:
        return REWARD_APPLE
    return REWARD_STEP


def run_episode(agent, board, batch_size: int) -> tuple[int, float]:
    state = grid_to_state(board)
    total_reward = 0.0

    while not board.is_game_over():
        action_idx = agent.select_action(state)
        move = action_idx_to_move(action_idx)

        score_before = board.get_score()
        try:
            board.apply_move(move)
        except ValueError:
            pass

        done = board.is_game_over()
        reward = compute_reward(score_before, board.get_score(), done)
        next_state = grid_to_state(board)

        agent.store_experience(state, action_idx, reward, next_state, done)
        agent.train(batch_size)

        state = next_state
        total_reward += reward

    return board.get_score(), total_reward
