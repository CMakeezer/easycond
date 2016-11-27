cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(easy-conditional)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(CXXFeatures)

if (CMAKE_COMPILER_IS_GNUCC OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(-std=gnu++17)
else()
    message(FATAL_ERROR "C++17 support is required")
endif()

check_cxx_if_constexpr()


configure_file(config.h.in config.h @ONLY)

add_subdirectory(tests)

set(easycond_HDRS
    easycond.h
)

install(FILES ${easycond_HDRS}
        DESTINATION include/EasyCond/
)

