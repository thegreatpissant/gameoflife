Game of life c++ version

Did my own implementation then I stumbled upon [Abrash's book](https://www.jagregory.com/abrash-black-book/#chapter-17-the-game-of-life)

- 'a' - toggle render using alive cell determiniation, default is on
- 'v' - toggle vsync, default is on
- 's' - print out stats of main loop
- 'z' - randomly populate alive cells
- 'x' - create a grid of alive cells
- 'q' - quit
- 'space' - start/stop simulation
- 'mouse click' - set cell as alive

Requires SDL3

Hit pause, click around, hit 'z' or 'x', then unpause

```
cmake -S . -B build
cmake --build build
./build/life
```
