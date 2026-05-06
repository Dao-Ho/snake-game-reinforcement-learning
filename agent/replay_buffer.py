import random
from collections import deque
from models import Experience

class ReplayBuffer():
    def __init__(self, max_len: int):
        self.replay_buffer = deque(maxlen = max_len)

    def __len__(self) -> int:
        return len(self.replay_buffer)

    def push(self, experience: Experience) -> None:
        deque.append(experience)

    def sample(self, size: int) -> list[Experience]:
        if size > len(self.replay_buffer):
            raise Exception("Sample size given must be less than the size of the replay buffer")
        return random.sample(self.replay_buffer, size)

        
        