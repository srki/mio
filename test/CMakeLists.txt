add_executable(main main.c)
target_link_libraries(main PRIVATE mio)
target_compile_definitions(main PRIVATE INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs")