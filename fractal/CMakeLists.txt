include_directories(include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")
add_library(FRACTAL ${SOURCES})

add_subdirectory("libs/glad")
list(APPEND EXTRA_LIBS GLAD)

add_subdirectory(libs/imgui)
list(APPEND EXTRA_LIBS IMGUI)

#Add GLFW
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("libs/glfw-3.3.8")
list(APPEND EXTRA_LIBS glfw)

add_subdirectory("libs/glm")
list(APPEND EXTRA_LIBS glm::glm)
include_directories("libs/glm/glm")

project(FRACTAL VERSION 1.0)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

target_link_libraries(FRACTAL PUBLIC ${EXTRA_LIBS})
target_include_directories(FRACTAL
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


install(TARGETS FRACTAL DESTINATION lib)
install(DIRECTORY include DESTINATION include)