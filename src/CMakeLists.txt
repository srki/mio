add_library(mio
        ../external/mmio/mmio.c
        reader.c
        writer.c)

target_include_directories(mio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
target_link_libraries(mio PRIVATE mmio)