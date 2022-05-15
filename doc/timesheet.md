# Timesheet

| Time | Date       | Work done                                           |
| ----:| ---------- | --------------------------------------------------- |
| 2h   | 2022-04-05 | Setup project repo and buildtools, add Logger class |
| 1h   | 2022-04-06 | Colorize log messages based on level |
| .5h  | 2022-04-06 | Add CMake generated configuration header |
| .5h  | 2022-04-06 | Configure CMake to fetch & build SDL2 |
| 1h   | 2022-04-06 | Configure tests, add tests for Logger |
| 2h   | 2022-04-06 | Setup SDL2, draws bmp-images in a window |
| 2h   | 2022-04-07 | Abstract SDL2 |
| 2h   | 2022-04-10 | Configure buildtools, SDL2, SDL2-image, SDL2-ttf |
| 1.5h | 2022-04-11 | Create input class, handles keyboard & supports callbacks |
| .5h  | 2022-04-12 | Abstract SDL events and keycodes |
| 3h   | 2022-04-12 | Fix buildsystem, link to built libraries |
| 3h   | 2022-04-13 | ResourceManager, loads&serves fonts. SDL2 abstraction, draws menu text labels as textures |
| 3h   | 2022-04-14 | ResourceManager, loads&serves images. More abstraction: Color, Constants, Font, Helpers (hash function), Images |
| 4h   | 2022-04-18 | Game, Gamestates, Menusystem implementation, helper classes, tests for Dimensions2D logical op. overloads, Cleanup code |
| 3h   | 2022-04-19 | Implement game states: MENU, PAUSED (With menu), RUNNING (loop that sleeps), QUIT |
| 1h   | 2022-04-19 | Refactor Renderer, Texture. Add wrappers for sdl draw functions. Add class Rectangle |
| .5h  | 2022-04-19 | Add Timer class |
| .5h  | 2022-04-21 | Add GameObject class skeleton |
| 2h   | 2022-04-23 | Extend Timer class, add classes Timestep and Gameloop timer with tests |
| 2h   | 2022-04-24 | Extend Timetools module, add class TimeEstimate which implements Welford's online algorithm that computes an estimation, PreciseSleep function |
| 1h   | 2022-04-26 | Add GLM library, start working on gameplay |
| 3h   | 2022-04-28 | Add Physics engine with tests |
| 2h   | 2022-05-01 | Refactor physics, gameobject transform, drawing |
| 2h   | 2022-05-02 | Work on physics & gameplay |
| 2h   | 2022-05-03 | Add SDL-mixer to build system, reconfigure build system (less warnings from included 3rd party libs) |
| 2h   | 2022-05-03 | Search for assets, check their licenses and include them in the build |
| 2h   | 2022-05-03 | Add classes: Mixer, Music and Sound. Play music in menus&game + initial jumping sound |
| 2h   | 2022-05-07 | Implement InputComponent, handles input for GameObjects |
| 2h   | 2022-05-08 | Add GameLevel, refactor game states handling |
| 2h   | 2022-05-08 | Refactor GameObject InputComponent, uses proper Command pattern, Add mapping for objectkeycallback to input class |
| 1h   | 2022-05-09 | Study c++ macros & use macros to conditionally inclue profiling code in main game loop, refactor callbacks |
| 1h   | 2022-05-09 | Refactor GameObject to be a base class. Add PlayerObject extending it to represent player |
| 1h   | 2022-05-11 | Add infinitely scrolling background |
| 2h   | 2022-05-12 | Research game cameras, implement camera class and center player in the middle of the x-axis. Minor refactoring to other classes |
| 3h   | 2022-05-15 | Refactor and fix scaling issues, still some left. Add generation of map obstacles |
| 2h   | 2022-05-15 | Implement colissions between player and objects |
| 5h   | 2022-05-15 | Major refactoring, implemented states for player. Added help menu and some random gizmos to the gameplay
| 70h  |            | Total time spent |

