cmake_minimum_required(VERSION 3.0)
project(Simple-Game)

set(CMAKE_BUILD_TYPE "Debug")

set(GNU_CLANG ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
if(GNU_CLANG)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc -mwindows")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Add sub-directories
add_subdirectory(Engine)

# Includes
AUX_SOURCE_DIRECTORY(src SRC)

include_directories("src")
include_directories("Engine/src")

add_executable(${PROJECT_NAME} ${SRC})

#link with engine
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

target_link_libraries(${PROJECT_NAME} Engine)

set(CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
