from pydantic import BaseModel

class Experience(BaseModel):
    state: list[float]
    action: int
    reward: float
    next_state: list[float]
    done: bool
    