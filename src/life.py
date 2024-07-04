"""Life implementation
"""
import math
import random
import pygame
from libs import atom as Atom
import cProfile


def draw(surface: pygame.Surface,
         pos_x: float, pos_y: float,
         color: str):
    pygame.draw.line(surface, color, start_pos=(pos_x, pos_y), end_pos=(pos_x+1, pos_y + 1), width=1)


def draw_atom(surface: pygame.Surface,
              atom: Atom.Atom):
    draw(surface, atom.pos_x, atom.pos_y, atom.color)


def generate_atoms(count: int, bounds: tuple[int, int]) -> list[Atom.Atom]:
    # generate {count} atoms with random attributes within bounds
    bounds_attaction: float = -1
    atoms = [Atom.Atom(
            pos_x=random.random()* bounds[0],
            pos_y=random.random()* bounds[1],
            attraction=random.random() *bounds_attaction,
            color="red"
        )
        for i in range(count)
    ]
    return atoms


def simulate(atoms: list[Atom.Atom], bounds: tuple[float, float], dt: float):
    for i in range(len(atoms)):
        force_x = 0.0
        force_y = 0.0
        for j in range(len(atoms)):
            if j == i:
                continue
            distance_x = atoms[i].pos_x - atoms[j].pos_x
            distance_y = atoms[i].pos_y - atoms[j].pos_y
            distance = (distance_x * distance_x + distance_y * distance_y) ** 0.5
            force = 0
            if distance > 0:
                force = (atoms[i].attraction + atoms[j].attraction) / distance ** 2
            force_x += distance_x * force
            force_y += distance_y * force
        def bounds_1():
            if atoms[i].pos_x < 0:
                atoms[i].pos_x += bounds[0]
            elif atoms[i].pos_x > bounds[0]:
                atoms[i].pos_x -= bounds[0]
            if atoms[i].pos_y < 0:
                atoms[i].pos_y += bounds[1]
            elif atoms[i].pos_y > bounds[1]:
                atoms[i].pos_y -= bounds[1]
        def bounds_2():
            if atoms[i].pos_x < 0:
                atoms[i].velocity_x = abs(atoms[i].velocity_x)
            elif atoms[i].pos_x > bounds[0]:
                atoms[i].velocity_x = abs(atoms[i].velocity_x) * -1
            if atoms[i].pos_y < 0 or atoms[i].pos_y > bounds[1]:
                atoms[i].velocity_y *= -1
        def bounds_3():
            if atoms[i].pos_x < 0:
                atoms[i].pos_x = bounds[0]
            elif atoms[i].pos_x > bounds[0]:
                atoms[i].pos_x = 0
            if atoms[i].pos_y < 0:
                atoms[i].pos_y = bounds[1]
            elif atoms[i].pos_y > bounds[1]:
                atoms[i].pos_y = 0
        atoms[i].velocity_x += force_x * dt * 0.5
        atoms[i].velocity_y += force_y * dt * 0.5

        # atoms[i].velocity_x *= 0.99
        # atoms[i].velocity_y *= 0.99

        atoms[i].pos_x += atoms[i].velocity_x
        atoms[i].pos_y += atoms[i].velocity_y
        bounds_3()


def interpolate_atoms(atoms: list[Atom.Atom], alpha: float):
    #  Interpolate atom positions based on an alpha
    o_alpha = 1 - alpha
    for atom in atoms:
        atom.pos_x = atom.previous_pos_x * o_alpha + atom.pos_x * alpha
        atom.pos_y = atom.previous_pos_y * o_alpha + atom.pos_y * alpha

def main():
    window_h: float = 600.0
    window_w: float = 600.0
    num_atoms: int = 500
    window_dimensions: tuple[float, float] = (window_w, window_h)
    pygame.init()
    window = pygame.display.set_mode(window_dimensions)
    atoms = generate_atoms(num_atoms, window_dimensions)
    run_simulation = True
    clock = pygame.time.Clock()
    target_fps = 10
    simulaton_tick_rate = 15
    simulation_time = 0.0
    time_since_last_simulation = 0.0
    while run_simulation:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run_simulation = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    run_simulation = False
                if event.key == pygame.K_p:
                    print(atoms)
        window.fill(0)
        for atom in atoms:
            atom.previous_pos_x = atom.pos_x
            atom.previous_pos_y = atom.pos_y

        dt = clock.tick(target_fps) / 1000.0
        time_since_last_simulation += dt
        while time_since_last_simulation >= 1.0 / simulaton_tick_rate:
            simulate(atoms, window_dimensions, 1.0 / simulaton_tick_rate)
            simulation_time += 1.0 / simulaton_tick_rate
            time_since_last_simulation -= 1.0 / simulaton_tick_rate
        alpha = time_since_last_simulation * simulaton_tick_rate
        interpolate_atoms(atoms, alpha)
        for atom in atoms:
            draw_atom(window, atom)
        pygame.display.flip()

    pygame.quit()


if __name__ == "__main__":
    main()
    # cProfile.run('main()')


