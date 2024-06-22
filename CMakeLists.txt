cmake_minimum_required(VERSION 3.18.4)
project(Lab_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(A3 main.c
        main.h
        load.c
        load.h
        decode.c
        decode.h
        entries.c
        entries.h
        memory.c
        memory.h
        debug.c
        debug.h
        run.c
        run.h
        execute.c
        execute.h
)
