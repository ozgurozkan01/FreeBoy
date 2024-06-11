cmake_minimum_required(VERSION 3.27)
project(FreeBoy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/freeboy/cmake)

find_package(SDL2 CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
add_executable(FreeBoy main.cpp
        freeboy/src/Cartridge.cpp
        freeboy/include/Cartridge.h
        freeboy/src/GameBoy.cpp
        freeboy/include/GameBoy.h
        freeboy/include/util/Utility.h
        freeboy/src/CPU.cpp
        freeboy/include/CPU.h
        freeboy/src/InstructionSet.cpp
        freeboy/include/InstructionSet.h
        freeboy/src/Registers.cpp
        freeboy/include/Registers.h
)

target_link_libraries(FreeBoy ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows)
