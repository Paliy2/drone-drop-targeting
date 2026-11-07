cmake_minimum_required(VERSION 3.16)
project(DropDropTargeting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(ENABLE_SANITIZERS "Build with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)

add_library(drop_core STATIC core/src/drop_planner.cpp)
target_include_directories(drop_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core/include)
target_compile_options(drop_core PRIVATE -Wall -Wextra)

add_executable(drop_calc app/main.cpp)
target_compile_options(drop_calc PRIVATE -Wall -Wextra)
target_link_libraries(drop_calc PRIVATE drop_core)

if(ENABLE_SANITIZERS)
    target_compile_options(drop_core PUBLIC
        -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined -g)
    target_link_options(drop_core PUBLIC -fsanitize=address,undefined)
endif()
