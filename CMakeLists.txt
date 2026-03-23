cmake_minimum_required(VERSION 3.20)
project(CPP)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined -g -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,leak,undefined")

add_executable(CPP main.cpp
        String/String-v26.h
        String/String-v17.h
        Vector/Vector-v26.h
        Vector/Vector-v17.h
        SmartPointer/Uniqueptr.h
        SmartPointer/Sharedptr.h
)
