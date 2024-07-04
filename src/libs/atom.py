from dataclasses import dataclass
@dataclass
class Atom:
    pos_x: float = 0.0
    pos_y: float = 0.0
    velocity_x: float = 0.0
    velocity_y: float = 0.0
    attraction: float = 0.0
    color: str = ""
    previous_pos_x: float = 0.0
    previous_pos_y: float = 0.0