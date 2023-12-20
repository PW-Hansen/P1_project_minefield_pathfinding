# P1 G16: Minefield Clearing

Cloning to new folder: `git clone --recurse-submodules git@github.com:PW-Hansen/P1_project_minefield_pathfinding.git`

If downloaded from another location, run `git submodule update --recursive --init` instead.

### HOWTO Compile

Either you can use Nix: `nix build`, or you can use CMake directly. To do so, run the following commands at the root of the project:
```bash
cmake -S . -B build # Generates the makefiles
make -C build # (or `cmake --build build`) to build the binaries
```

### HOWTO Test

Tests are automatically built and run in `nix build`.

To run manually, follow the CMake compile step, then run the following from the root of the project:
```bash
make test -C build # (or `ctest --test-dir build`)
```

### HOWTO Run

Either use Nix: `nix run`, or use CMake's HOWTO Compile above. Then run the following:

```bash
./build/src/main
```

## Run the examples

To run the example map, input the following:
```
static/testmap.ppm
static/hotspot_map.ppm
```

The project goal will be to determine the cheapest path possible between two cities for road-building purposes, including mineclearing costs if any minefields are present along the chepest path.

Note, the values below assume the settings as included in the project originally (`src/settings.h`). Modified settings may give different results.

If you input the following, you should be advised that you do not go ahead with the project.
```
GDP City 1: 50
GDP City 2: 100
```

Alternatively, you can input this instead, which should now advice to do the project, as described by the pathfinding algorithm.
```
GDP City 1: 50
GDP City 2: 500
```
