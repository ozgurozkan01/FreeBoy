cmake_minimum_required(VERSION 3.27)
project(FreeBoy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
add_executable(FreeBoy main.cpp
        src/memory/Cartridge.cpp
        src/memory/Cartridge.h
        src/gameboy/GameBoy.cpp
        src/gameboy/GameBoy.h
        src/graphics/Graphic.cpp
        src/graphics/Graphic.h
        src/memory/Bus.cpp
        src/memory/Bus.h
)

target_link_libraries(FreeBoy ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows)
