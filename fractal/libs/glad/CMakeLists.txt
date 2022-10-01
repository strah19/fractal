include_directories(include)

file(GLOB SOURCES "src/*.c")
add_library(GLAD ${SOURCES})

target_include_directories(GLAD
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


install(TARGETS GLAD DESTINATION lib)
install(DIRECTORY include DESTINATION include)