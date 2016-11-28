cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(easy-conditional)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_COMPILER_IS_GNUCC OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(-std=gnu++14)
else()
    message(FATAL_ERROR "C++14 support is required")
endif()

add_subdirectory(tests)

set(easycond_HDRS
    easycond.h
)

install(FILES ${easycond_HDRS}
        DESTINATION include/EasyCond/
)

