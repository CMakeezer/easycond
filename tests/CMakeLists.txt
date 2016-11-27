include_directories(${CMAKE_SOURCE_DIR})

add_executable(easycond_test test.cpp)
add_test(easycond_test easycond_test)
