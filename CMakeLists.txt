cmake_minimum_required(VERSION 3.10)

project(SANDBOX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_subdirectory(FRACTAL)
list(APPEND EXTRA_LIBS FRACTAL)

add_executable(SANDBOX ${SOURCES})
target_link_libraries(SANDBOX PUBLIC ${EXTRA_LIBS})
target_include_directories(SANDBOX PUBLIC "${PROJECT_BINARY_DIR}/include")

add_custom_command(TARGET SANDBOX PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:SANDBOX>)

install(TARGETS SANDBOX DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/include/config.h"
        DESTINATION include)

include(CTest)
