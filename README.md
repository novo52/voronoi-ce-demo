## Recursive Rendering Voronoi Demo

Draws a simple rasterized voronoi graph rendered with progressive refinement until the native resolution of the screen is reached.

Written to run on the TI-84 Plus CE graphing calculator.

### Output

Running the program

!["asm(prgmDEMO)" is entered into the console. The screen changes to show a multicoloured voronoi graph which becomes higher resolution over time.](progressive_render_demo.gif "Program output")

Time-constant frames

![The same voronoi graph, but each frame is shown on screen for the same time period.](constant_output.gif "Time-adjusted program output")

### Downloading and Running

1. Download the latest release from the releases tab
2. Download the latest release of CLibs from [here](https://github.com/CE-Programming/libraries/releases/latest)
3. Upload `DEMO.8xp` and `clibs.8xg` to your calculator
4. Run `asm(prgmDEMO)`


### Building

Requires the [CE C/C++ Toolchain](https://github.com/CE-Programming/toolchain)

make using `cedev.bat`
