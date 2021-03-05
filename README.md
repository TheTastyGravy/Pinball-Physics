# Pinball physics game
A pinball game made using a custom physics engine in C++, and AIE bootstrap for graphics

## Building
Bootstrap is rather error prone, so building can be problematic. When building bootstrap, one configuration will cause a linker error, so try both if one doesnt work. Beyond that, errors are more unique and have to be solved on a case-by-case basis.

After building bootstrap, the PhysicsProject2D project can be built, containing the custom physics engine and pinball game.

## Usage
The custom physics engine consists of the PhysicsScene class, handling updating objects, collision detection, and applying contact forces, and classes dirived from PhysicsObject, notably Rigidbody, implementing collision resolution and object movement.

For further explanation of how the physics engine works and the logic for the pinball game, reference the documentation.

### How to play
A and D control the flippers, holding space pulls the launcher down to put a ball into play. Pressing P displays all controls.
After loosing 3 balls, its game over.
