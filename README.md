# game-project-course
A mysterious project for the course called Game Project Course.

## Requirements
* C++17 Compiler
* GNU Make
* CMake ~3.16 or newer

## External libraries used
* [{fmt}](https://fmt.dev/latest/index.html)
* [SDL2](https://www.libsdl.org/)
* [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
* [SDL_image](https://www.libsdl.org/projects/SDL_image/)
* [Google Test](https://github.com/google/googletest)


## Building and running
CMake is used as the build system. You can set up the build system and build the project with the provided scripts. Cmake will download and build all the required libraries.

There are 2 different build targets configured:
- Debug - Default target which builds an unoptimized version of the project with debug symbols included. This target will also build the tests.
- Release - This target will build an optimized version without the tests or debug symbols.

```console
foo@bar:dir$ git clone git@github.com:kordaniel/game-project-course.git
foo@bar:dir$ cd game-project-course
foo@bar:game-project-course$ ./configure.sh [r]
foo@bar:game-project-course$ ./build.sh [r]
foo@bar:game-project-course$ ./runtests.sh [c]
foo@bar:game-project-course$ ./bin/gameproj[-dbg]
```
Optional arguments:
* `r` to target the optimized Release target.
* `c` to run the tests with [ctest](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html), which creates a short overview of the test results instead of running all tests individually.  

Binaries:
```console
/bin/gameproj     - Release build
/bin/gameproj-dbg - Debug build
/bin/*Test        - Various tests
```
