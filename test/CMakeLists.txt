add_executable(mio-main main.c)
target_link_libraries(mio-main PRIVATE mio)
target_compile_definitions(mio-main PRIVATE INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/inputs")
