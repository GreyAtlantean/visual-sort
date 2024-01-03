# CMakeLists.txt for Visual_Sort project in C

cmake_minimum_required(VERSION 3.10)
project(visual_sort C)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_COMPILER clang)

# Find SDL2 package
#find_package(SDL2 REQUIRED)
#find_package(SDL2_TTF REQUIRED)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Set source files using file(GLOB ...)
set(SOURCE_FILES "main.c")

# Add executable target with compile options
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PUBLIC
    -Wall
    -Wempty-body
	#-Werror
	#-Wstrict-prototypes
    -Werror=uninitialized
    -Warray-bounds
    -g
	#-pg
	#-Ofast
	-O3
	-lprofiler
)

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
