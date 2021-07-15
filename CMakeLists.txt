cmake_minimum_required(VERSION 3.13)
project(proiect)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall --pedantic -I/usr/local/include/fuse3 -L/usr/local/lib/x86_64-linux-gnu -lfuse3 -lpthread -g")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 14)

find_package(FUSE REQUIRED)

include_directories("/usr/local/include/fuse3")

add_executable(server
        glu_globals.h
        glu_responses.c
        glu_responses.h
        server.c)
add_executable(client
        glu_globals.h
        glu_requests.c
        glu_requests.h
        client_f.c)

target_link_libraries(client /usr/lib/x86_64-linux-gnu)
target_link_libraries(server ${FUSE_LIBRARIES})

#add_executable(proiect
#    client_f.c
#    glu_globals.h
#    glu_requests.c
#    glu_requests.h
#    glu_responses.c
#    glu_responses.H
#    passthrough_fh_c.c
#    server.c)
