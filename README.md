# game-project-course
A mysterious project for the course called Game Project Course.

## Requirements
* C++17 Compiler
* GNU Make
* CMake ~3.16 or newer

## External libraries used
* [{fmt}](https://fmt.dev/latest/index.html)
* [OpenGL Mathematics (GLM)](https://github.com/g-truc/glm)
* [SDL2](https://www.libsdl.org/)
* [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
* [SDL_image](https://www.libsdl.org/projects/SDL_image/)
* [SDL_mixer](https://libsdl.org/projects/SDL_mixer/)
* [Google Test](https://github.com/google/googletest)


## Building and running
CMake is used as the build system. You can set up the build system and build the project with the provided scripts. Cmake will download and build all the required libraries. Cmake will also copy all the required resources into paths inside the output directory `./bin/res/` so always make sure to execute the binaries from the output directory.

There are 2 different build targets configured:
- Debug - Default target which builds an unoptimized version of the project with debug symbols included. This target will also build the tests.
- Release - This target will build an optimized version without the tests or debug symbols.

### Binaries
```console
/bin/gameproj     - Release build
/bin/gameproj-dbg - Debug build
/bin/*Test        - Various tests
```

### Steps to take
```console
foo@bar:dir$ git clone git@github.com:kordaniel/game-project-course.git
foo@bar:dir$ cd game-project-course
foo@bar:game-project-course$ ./configure.sh [r] - Setup the build system
foo@bar:game-project-course$ ./build.sh [r]     - Build the project
foo@bar:game-project-course$ ./runtests.sh [c]  - Run all tests (Debug builds)
foo@bar:game-project-course$ ./run.sh [r]       - Execute the project
OR
foo@bar:game-project-course$ cd bin/; ./bin/gameproj[-dbg]
```
Optional arguments:
* `r` to target the optimized Release target.
* `c` to run the tests with [ctest](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html), which creates a short overview of the test results instead of running all tests individually.

## Assets
| Asset | License |
| ----- | ------- |
| [41 Short, Loopable Background Music Files](https://joshuuu.itch.io/short-loopable-background-music) | "Free for personal or commercial use as long as you don't redistribute as your own. Donations also welcome! Please add **jhaeka** to your credits. :)" |
| [8 bit SFX pack](https://maskedsound.itch.io/8-bit-sfx-pack) | "free" [- @Maskedsound](https://twitter.com/maskedsound) |
