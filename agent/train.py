from collections import deque

import snake_engine_py as snake_engine

import checkpoint
from agent import SnakeGameAgent
from env_wrapper import run_episode

WIDTH = 24
HEIGHT = 21
EPISODES = 1000
BATCH_SIZE = 32
CHECKPOINT_EVERY = 100

agent = SnakeGameAgent()
recent_scores = deque(maxlen=100)
best_avg = float('-inf')

for episode in range(EPISODES):
    board = snake_engine.Board(WIDTH, HEIGHT)
    score, total_reward = run_episode(agent, board, BATCH_SIZE)

    recent_scores.append(score)
    avg_score = sum(recent_scores) / len(recent_scores)

    print(f"Episode {episode:4d} | Score: {score:3d} | "
          f"Avg(100): {avg_score:5.2f} | Reward: {total_reward:7.2f} | "
          f"Epsilon: {agent.epsilon:.3f}")

    if episode % CHECKPOINT_EVERY == 0 and episode > 0:
        checkpoint.save(agent, f'episode_{episode}')

    if avg_score > best_avg and len(recent_scores) == recent_scores.maxlen:
        best_avg = avg_score
        checkpoint.save(agent, 'best')

checkpoint.save(agent, 'final')
