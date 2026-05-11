import os
import torch

CHECKPOINT_DIR = os.path.join(os.path.dirname(__file__), 'checkpoints')


def save(agent, name: str) -> str:
    os.makedirs(CHECKPOINT_DIR, exist_ok=True)
    path = os.path.join(CHECKPOINT_DIR, f'{name}.pth')
    torch.save({
        'model_state': agent.model.state_dict(),
        'epsilon': agent.epsilon,
    }, path)
    return path


def load(agent, name: str) -> None:
    path = os.path.join(CHECKPOINT_DIR, f'{name}.pth')
    state = torch.load(path)
    agent.model.load_state_dict(state['model_state'])
    agent.epsilon = state['epsilon']
