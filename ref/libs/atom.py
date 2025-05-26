import pygame
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


def draw(atom: Atom, surface: pygame.Surface):
    pygame.draw.line(surface, atom.color, start_pos=(atom.previous_pos_x, atom.previous_pos_y), end_pos=(atom.pos_x, atom.pos_y), width=1)
