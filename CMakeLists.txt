cmake_minimum_required(VERSION 3.16)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(3DVectorVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
    GIT_SHALLOW ON
)
set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
FetchContent_MakeAvailable(SFML)

add_executable(visualizer
    src/main.cpp
    src/vector.cpp
    src/plane.cpp
    src/to_string.cpp
)

target_compile_definitions(visualizer PRIVATE SFML_STATIC)
target_link_libraries(visualizer PRIVATE sfml-graphics sfml-window sfml-system)
target_link_options(visualizer PRIVATE -static-libgcc -static-libstdc++)