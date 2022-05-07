cmake_minimum_required(VERSION 3.10)
project(RayTracing VERSION 0.1.0)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR})

include(Dependencies.cmake)
include(Utils.cmake)

include(CTest)
enable_testing()

file(GLOB_RECURSE SRC_FILES ${SOURCE_DIR}/src/*.cpp ${SOURCE_DIR}/src/*.h)
add_executable(RayTracing ${SRC_FILES})

target_include_directories(RayTracing PRIVATE ${GRAPHICS_INCLUDE_DIRS})

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/Glad)

target_link_libraries(RayTracing PRIVATE ${GRAPHICS_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
