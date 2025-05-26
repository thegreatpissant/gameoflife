"""Life implementation
"""
import secrets

import pygame
from libs import atom as Atom
from libs import physics as Physics


def draw(surface: pygame.Surface,
         pos_x: float, pos_y: float,
         color: str):
    pygame.draw.line(surface, color, start_pos=(pos_x, pos_y), end_pos=(pos_x+1, pos_y + 1), width=1)


def generate_atoms(count: int, bounds: tuple[float, float], attraction: float, color: str) -> list[Atom.Atom]:
    sys_rand = secrets.SystemRandom()
    atoms = [Atom.Atom(
            pos_x=bounds[0] / 4 + sys_rand.random() * bounds[0]/2,
            pos_y=bounds[1] / 4 + sys_rand.random() * bounds[1]/2,
            attraction=attraction,
            color=color
        )
        for i in range(count)
    ]
    return atoms


def main():
    window_d = 1500
    window_h: float = window_d
    window_w: float = window_d
    window_dimensions: tuple[float, float] = (window_w, window_h)

    pygame.init()
    screen = pygame.display.set_mode(window_dimensions)

    num_atoms: int = 50
    red_atoms = generate_atoms(num_atoms, window_dimensions, -.2, "red")
    green_atoms = generate_atoms(num_atoms, window_dimensions, -.1, "green")
    clock = pygame.time.Clock()
    target_fps = 30
    bound_function = 0
    run_simulation = True
    while run_simulation:
        dt = clock.tick(target_fps) / 1000.0
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run_simulation = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    run_simulation = False
                if event.key == pygame.K_p:
                    print(red_atoms + green_atoms)
                if event.key == pygame.K_b:
                    bound_function = (1 + bound_function) % len(Physics.bound_functions)
                    print(f'Using bound function: {Physics.bound_functions[bound_function].__name__}')
        screen.fill(0)
        Physics.simulate(red_atoms, red_atoms, dt)
        Physics.simulate(green_atoms, red_atoms, dt)
        Physics.update(green_atoms, window_dimensions, Physics.bound_functions[bound_function])
        Physics.update(red_atoms, window_dimensions, Physics.bound_functions[bound_function])
        for atom in red_atoms:
            Atom.draw(atom, screen)
        for atom in green_atoms:
            Atom.draw(atom, screen)
        pygame.display.flip()
    clock.tick()
    pygame.quit()


if __name__ == "__main__":
    main()


