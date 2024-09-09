cmake_minimum_required(VERSION 3.5)

project(Baccarat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} include)


file(CREATE_LINK
    ${PROJECT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
    SYMBOLIC)

# Glob all .cpp files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Glob all .h files in the include directory
file(GLOB_RECURSE HEADERS "include/*.h")

# Exclude CMake's internal files from being compiled
list(FILTER SOURCES EXCLUDE REGEX "/CMakeFiles/")

add_executable(Baccarat
    ${SOURCES}
    ${INCLUDE}
    )

target_link_libraries(Baccarat -lSDL2 -lSDL2_image -lSDL2_ttf)
