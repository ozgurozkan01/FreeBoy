cmake_minimum_required(VERSION 3.27)
project(FreeBoy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/freeboy/cmake)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(FreeBoy main.cpp
        freeboy/src/Cartridge.cpp
        freeboy/include/Cartridge.h
        freeboy/src/GameBoy.cpp
        freeboy/include/GameBoy.h
        freeboy/src/CPU.cpp
        freeboy/include/CPU.h
        freeboy/src/InstructionSet.cpp
        freeboy/include/InstructionSet.h
        freeboy/src/Registers.cpp
        freeboy/include/Registers.h
        freeboy/src/MMU.cpp
        freeboy/include/MMU.h
        freeboy/src/ALU.cpp
        freeboy/include/ALU.h
        freeboy/include/InterruptHandler.h
        freeboy/src/IO.cpp
        freeboy/include/IO.h
        freeboy/src/PPU.cpp
        freeboy/src/Joypad.cpp
        freeboy/include/Joypad.h
        freeboy/src/InterruptHandler.cpp
        freeboy/src/Timer.cpp
        freeboy/include/Timer.h
        freeboy/src/DMA.cpp
        freeboy/include/DMA.h
        freeboy/src/Renderer.cpp
        freeboy/include/Renderer.h
        freeboy/src/LCD.cpp
        freeboy/include/LCD.h
)

target_link_libraries(FreeBoy ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows SDL2_ttf::SDL2_ttf)
find_package(SDL2_ttf CONFIG REQUIRED)
