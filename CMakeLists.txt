cmake_minimum_required(VERSION 2.8)
project(Simple-Game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o0 -g3 -std=c++0x -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Add sub-directories
add_subdirectory(Engine)

# Includes
AUX_SOURCE_DIRECTORY(src SRC)

include_directories("src")
include_directories("Engine/src")

add_executable(${PROJECT_NAME} ${SRC})

#link with engine
target_link_libraries(${PROJECT_NAME} Engine)
