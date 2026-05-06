import random
from collections import deque
from models import Experience

class RelayBuffer():
    def __init__(self, max_len: int):
        self.max_len = max_len
        self.relay_buffer = deque(maxlen = max_len)

    def __len__() -> int:
        return len(self.relay_buffer)

    def push(experience: Experience) -> None:
        deque.appendLeft(experience)

    def sample(size: int) -> list[Experience]:
        if size > self.max_len:
            raise Exception("Sample size given must be less than the size of the relay buffer")
        return random.sample(self.relay_buffer, size)
        
        
        
        