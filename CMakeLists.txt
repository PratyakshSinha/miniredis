cmake_minimum_required(VERSION 3.16)
project(miniredis VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server
    src/main.cpp
    src/server.h
    src/server.cpp
    src/store.cpp
    src/store.h
)

add_executable(client
    client/main.cpp
)

target_include_directories(server PRIVATE src)

find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads ws2_32)
target_link_libraries(client PRIVATE ws2_32)
