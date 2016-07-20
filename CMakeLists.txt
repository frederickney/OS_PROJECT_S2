cmake_minimum_required(VERSION 3.2)
project(OS_PROJECT_S2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=gnu99")
set(EXE "ftp_mqtt")
set(HOST "HOST")
set(SOURCE_FILES
    src/cmd_interpreter.c
    src/cmd_interpreter.h
    src/file_access.c
    src/file_access.h
    src/file_opener.c
    src/file_opener.h
    src/get_request.c
    src/get_request.h
    src/main.c
    src/reply.c
    src/reply.h
    src/socket.c
    src/socket.h
    src/server.h
    src/server.c
    src/stdin_filter.c
    src/stdin_filter.h)

set(HOST_SOURCE_FILES
    src/onlineClient.c
    src/stdin_filter.c
    src/stdin_filter.h
    src/socket.c
    src/socket.h
    src/cmd_interpreter.c
    src/cmd_interpreter.h
    src/file_opener.c
    src/file_opener.h
    src/file_access.c
    src/file_access.h
    src/get_request.c
    src/get_request.h
    src/request.h
    src/request.c
    src/get_reply.h
    src/get_reply.c)

add_executable(${EXE} ${SOURCE_FILES})
add_executable(${HOST} ${HOST_SOURCE_FILES})