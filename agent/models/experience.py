from pydantic import BaseModel

class Experience(BaseModel):
    state: list[int]
    action: int 
    reward: float
    next_state: list[int]
    done: bool
    