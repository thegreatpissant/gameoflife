from ref.libs import atom


def wrap_around(atom: atom.Atom, bounds: tuple[float, float]):
    if atom.pos_x < 0:
        atom.pos_x += bounds[0]
    elif atom.pos_x > bounds[0]:
        atom.pos_x -= bounds[0]
    if atom.pos_y < 0:
        atom.pos_y += bounds[1]
    elif atom.pos_y > bounds[1]:
        atom.pos_y -= bounds[1]


def wall_bounce(atom: atom.Atom, bounds: tuple[float, float]):
    if atom.pos_x < 0:
        atom.velocity_x = abs(atom.velocity_x)
    elif atom.pos_x > bounds[0]:
        atom.velocity_x = abs(atom.velocity_x) * -1
    if atom.pos_y < 0:
        atom.velocity_y = abs(atom.velocity_y)
    elif atom.pos_y > bounds[1]:
        atom.velocity_y = abs(atom.velocity_y) * -1


def wraparound_clamp_to_boundry(atom: atom.Atom, bounds: tuple[float, float]):
    if atom.pos_x < 0:
        atom.pos_x = bounds[0]
    elif atom.pos_x > bounds[0]:
        atom.pos_x = 0
    if atom.pos_y < 0:
        atom.pos_y = bounds[1]
    elif atom.pos_y > bounds[1]:
        atom.pos_y = 0


def wall_bounce_negate_velocity(atom: atom.Atom, bounds: tuple[float, float]):
    if atom.pos_x < 0 or atom.pos_x > bounds[0]:
        atom.velocity_x *= -1
    if atom.pos_y < 0 or atom.pos_y > bounds[1]:
        atom.velocity_y *= -1


bound_functions = [
    wall_bounce,
    wall_bounce_negate_velocity,
    wrap_around,
    wraparound_clamp_to_boundry,
]


def simulate(atoms: list[atom.Atom], other_atoms: list[atom.Atom], dt: float):
    for i in range(len(atoms)):
        force_x = 0.0
        force_y = 0.0
        for j in range(len(other_atoms)):
            distance_x = atoms[i].pos_x - atoms[j].pos_x
            distance_y = atoms[i].pos_y - atoms[j].pos_y
            distance = (distance_x * distance_x + distance_y * distance_y) ** 0.5
            force = 0
            if distance > 0:
                force = (atoms[i].attraction ) / distance # ** 2
            # force = (atoms[i].attraction + atoms[j].attraction) / distance ** 2
            force_x += distance_x * force
            force_y += distance_y * force

        atoms[i].velocity_x += force_x * dt * 0.5
        atoms[i].velocity_y += force_y * dt * 0.5

        # atoms[i].velocity_x *= 0.99
        # atoms[i].velocity_y *= 0.99


def update(atoms: list[atom.Atom], bounds: tuple[float, float], bounds_operation):
    for atom in atoms:
        atom.previous_pos_x = atom.pos_x
        atom.previous_pos_y = atom.pos_y
        atom.pos_x += atom.velocity_x
        atom.pos_y += atom.velocity_y
        bounds_operation(atom, bounds)