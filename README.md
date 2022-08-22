# Raylib template with CMake and Github Actions

## Building to Web

**You need to install Emscripten** [Download Here!](https://emscripten.org/docs/getting_started/downloads.html)

1. Clone the repository using Git. Execute this in a terminal: `git clone https://github.com/siddharthroy12/RaylibTemplate`
1. Cd into RaylibTemplate. `cd RaylibTemplate`
1. Install the raylib git submodule using `git submodule update --init --recursive --depth=1`
1. Make a build folder. Your builds will go here. `mkdir build; cd build`
1. Setup cmake `emcmake cmake -S .. -D CMAKE_BUILD_TYPE=Release`
1. Run `cmake --build build` to compile the project
1. Run a local web server and open the RaylibTemplate.html

## Building (Linux/macOS)

1. Clone the repository using `git clone https://github.com/siddharthroy12/RaylibTemplate`
1. Move to the RaylibTemplate directory
`cd RaylibTemplate`
1. Install the raylib git submodule using `git submodule update --init --recursive --depth=1`
1. Make a build folder and cd to it with `mkdir build && cd build`
1. Setup cmake `cmake .. -DCMAKE_BUILD_TYPE=Release`
1. Run `make` to compile the project
1. Make the binary executable `chmod +x RaylibTemplate`
1. Run the binary with `./RaylibTemplate`

## Building (Windows, MinGW)

**You need to install MinGW. Select make and developer tools in the installer** [Download MinGW here!](https://sourceforge.net/projects/mingw/)

1. Clone the repository using Git. Execute this in a terminal: `git clone https://github.com/siddharthroy12/RaylibTemplate`
1. Cd into RaylibTemplate. `cd RaylibTemplate`
1. Install the raylib git submodule using `git submodule update --init --recursive --depth=1`
1. Make a build folder. Your builds will go here. `mkdir build; cd build`
1. Setup CMake. **Make sure to define MinGW Makefiles if you are using MinGW!** `cmake .. -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"`
1. Make sure you have mingw32-make. If you do, just run it in the build directory and it will start compiling everything. It won't take long, and once it's finished, then
1. Run RaylibTemplate.exe! You have just compiled RaylibTemplate for Windows using MinGW.

## Building (Windows, MSVC)

You need to have Visual Studio 2019 (other versions aren't tested, please tell us if it works for you) with C++ Development selected in the installer.

1. Clone the repository using Git. Execute this in a terminal: `git clone https://github.com/siddharthroy12/RaylibTemplate`
1. Cd into RaylibTemplate. `cd RaylibTemplate`
1. Install the raylib git submodule using `git submodule update --init --recursive --depth=1`
1. Make a build folder. Your builds will go here. `mkdir build; cd build`
1. Setup CMake. `cmake .. -DCMAKE_BUILD_TYPE=Release`
1. Let's build the project! Run `cmake --build .`
1. Go into Debug, your build of RaylibTemplate is there. You have now compiled RaylibTemplate for Windows using MSVC.
