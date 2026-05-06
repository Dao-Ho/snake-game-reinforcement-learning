from model import DQN
from replay_buffer import ReplayBuffer
import torch.optim as optim
import torch
import random

class SnakeGameAgent():
    def __init__(self):
        self.model = DQN(input_size=24*21, output_size=4)
        self.replay_buffer = ReplayBuffer(256)
        self.optimizer = optim.Adam(self.model.parameters(), lr=0.001)
        self.epsilon = 1.0
        self.epsilon_decay = 0.995
        self.epsilon_min = 0.01

    def select_action(self, state):
        with torch.no_grad():
            if random.random() < self.epsilon:
                # explore a random move that may not be optimal
                return random.randint(0, 3)
            else:
                q_values = self.model(state)
                return torch.argmax(q_values).item()
    