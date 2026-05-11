import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'snake-engine', 'build'))
import snake_engine_py as snake_engine

REWARD_APPLE = 10.0
REWARD_DEATH = -10.0
REWARD_STEP = -0.01


def action_idx_to_move(action_idx: int) -> snake_engine.Move:
    return [
        snake_engine.Move.kUp,
        snake_engine.Move.kDown,
        snake_engine.Move.kLeft,
        snake_engine.Move.kRight,
    ][action_idx]


def grid_to_state(board) -> list[float]:
    return [int(c) / 3.0 for c in board.get_grid()]


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
