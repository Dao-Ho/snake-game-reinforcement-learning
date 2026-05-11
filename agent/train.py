import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'snake-engine', 'build'))
import snake_engine_py as snake_engine
from agent import SnakeGameAgent

WIDTH = 24
HEIGHT = 21
EPISODES = 1000
BATCH_SIZE = 32

agent = SnakeGameAgent()

def action_idx_to_move(action_idx: int) -> snake_engine.Move:
    match action_idx:
        case 0:
            return snake_engine.Move.kUp
        case 1: 
            return snake_engine.Move.kDown
        case 2:
            return snake_engine.Move.kLeft
        case 3: 
            return snake_engine.Move.kRight
        
    

for episode in range(EPISODES):
    board = snake_engine.Board(WIDTH, HEIGHT)
    state = [int(c) for c in board.get_grid()]
    total_reward = 0

    while not board.is_game_over():
        action_idx = agent.select_action(state)
        move = action_idx_to_move(action_idx)
    
        score_before = board.get_score()
        try:
            board.apply_move(move)
        except ValueError:
            pass  # invalid move, treat as no-op
    
        score_after = board.get_score()
        done = board.is_game_over()
    
        if done:
            reward = -10
        elif score_after > score_before:
            reward = 10
        else:
            reward = -0.01
    
        next_state = [int(c) for c in board.get_grid()]
        agent.store_experience(state, action_idx, reward, next_state, done)
        agent.train(BATCH_SIZE)
    
        state = next_state
        total_reward += reward

    print(f"Episode {episode}: score={board.get_score()}, total_reward={total_reward}")