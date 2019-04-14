cmake_minimum_required(VERSION 3.10)
project(minecraft)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
aux_source_directory("src" SOURCES)
add_executable(minecraft ${SOURCES})

# find glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(minecraft ${GLM_LIBRARIES})

# add glad
add_subdirectory(glad)
target_link_libraries(minecraft glad)

# find glfw
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(minecraft ${GLFW_LIBRARIES})

