cmake_minimum_required(VERSION 3.27)
project(GameBoy_Emulator_SDL2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
add_executable(GameBoy_Emulator_SDL2 main.cpp
        src/memory/Cartridge.cpp
        src/memory/Cartridge.h
)

target_link_libraries(GameBoy_Emulator_SDL2 ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows)
