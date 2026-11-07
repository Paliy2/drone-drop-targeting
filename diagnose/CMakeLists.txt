cmake_minimum_required(VERSION 3.16)
project(DiagnoseCases LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_SANITIZERS "Build with AddressSanitizer + UBSan" OFF)

add_executable(case01 case-01-telemetry/main.cpp case-01-telemetry/sample_buffer.cpp)
add_executable(case02 case-02-budget/main.cpp case-02-budget/budget.cpp)
add_executable(case03 case-03-wind/main.cpp case-03-wind/wind.cpp)

foreach(target case01 case02 case03)
    target_compile_options(${target} PRIVATE -Wall -Wextra -g)
    if(ENABLE_SANITIZERS)
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endforeach()
