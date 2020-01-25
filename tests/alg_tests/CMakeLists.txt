include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(/home/luke/CLionProjects/alg/src)



macro(testing_unit)
    add_executable( testing_unit testing_unit.cc)
    target_link_libraries(testing_unit gtest gtest_main)

endmacro()

function(add_testing_unit NAME)
    add_executable( ${NAME} ${NAME}.cc)
    target_link_libraries(${NAME} gtest gtest_main)
endfunction(add_testing_unit)


add_testing_unit(fibonacci_heap_test)
add_testing_unit(max_queue_test)
add_testing_unit(priority_queue_test)
add_testing_unit(priority_suffix_test)
add_testing_unit(rolling_hash_test)
add_testing_unit(stack_test)
add_testing_unit(tree_algs_test)
add_testing_unit(tree_list_test)
add_testing_unit(vector_test)
add_testing_unit(suffix_array_test)

