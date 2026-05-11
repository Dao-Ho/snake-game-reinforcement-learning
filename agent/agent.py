from model import DQN
from replay_buffer import ReplayBuffer
from models.experience import Experience
import torch.optim as optim
import torch
import random
import torch.nn as nn

class SnakeGameAgent():
    def __init__(self):
        self.model = DQN(input_size=11, output_size=4)
        self.target_model = DQN(input_size=11, output_size=4)
        self.target_model.load_state_dict(self.model.state_dict())
        self.replay_buffer = ReplayBuffer(10000)
        self.optimizer = optim.Adam(self.model.parameters(), lr=0.001)
        self.epsilon = 1.0
        self.epsilon_decay = 0.995
        self.epsilon_min = 0.01
        self.gamma = 0.99
        self.target_update_freq = 100
        self.train_step = 0

    def select_action(self, state):
        with torch.no_grad():
            if random.random() < self.epsilon:
                # explore a random move that may not be optimal
                return random.randint(0, 3)
            else:
                state_tensor = torch.tensor(state, dtype=torch.float32)
                q_values = self.model(state_tensor)
                return torch.argmax(q_values).item()

    def store_experience(self, state, action, reward, next_state, done):
        self.replay_buffer.push(
            Experience(
                state = state, action = action, reward = reward, next_state = next_state, done = done
            )
        )

    def train(self, batch_size: int):
        # 1. Don't train if buffer doesn't have enough experiences yet
        if len(self.replay_buffer) < batch_size:
            return
    
        # 2. Sample a random batch
        batch = self.replay_buffer.sample(batch_size)
    
        # 3. Unpack batch into separate tensors
        states = torch.tensor([e.state for e in batch], dtype=torch.float32)
        actions = torch.tensor([e.action for e in batch], dtype=torch.long)
        rewards = torch.tensor([e.reward for e in batch], dtype=torch.float32)
        next_states = torch.tensor([e.next_state for e in batch], dtype=torch.float32)
        dones = torch.tensor([e.done for e in batch], dtype=torch.float32)
    
        # 4. Compute current Q-values: Q(s, a)
        current_q_values = self.model(states).gather(1, actions.unsqueeze(1))
    
        # 5. Compute target Q-values using Bellman:
        # target = reward + gamma * max(Q(s', a')) * (1 - done)
        # (1 - done) zeroes out future reward if game ended
        with torch.no_grad():
            next_q_values = self.target_model(next_states).max(1).values
        targets = rewards + self.gamma * next_q_values * (1 - dones)
    
        # 6. Compute loss between current and target Q-values
        loss = nn.MSELoss()(current_q_values.squeeze(), targets.detach())
    
        # 7. Backpropagate and update weights
        # optimizer.zero_grad() → loss.backward() → optimizer.step()
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        self.train_step += 1
        if self.train_step % self.target_update_freq == 0:
            self.target_model.load_state_dict(self.model.state_dict())

    def decay_epsilon(self) -> None:
        self.epsilon = max(self.epsilon_min, self.epsilon * self.epsilon_decay)