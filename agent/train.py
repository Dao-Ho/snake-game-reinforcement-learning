import sys
sys.path.append('/Users/dao/Documents/GitHub/snake-game-reinforcement-learning/snake-engine/build')
import snake_engine_py as snake_engine

board = snake_engine.Board(24, 21)
print(board.is_game_over())
print(board.get_score())
print(board.get_grid())