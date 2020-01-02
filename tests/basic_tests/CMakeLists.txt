include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(basic_check
        basic_check.cpp)

target_link_libraries(basic_check gtest gtest_main)