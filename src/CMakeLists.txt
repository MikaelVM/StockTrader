add_library(doctest OBJECT doctest.cpp)

add_executable(library_test library_test.cpp)
target_link_libraries(library_test PRIVATE doctest)
add_executable(stockReader_test stockReader_test.cpp)
target_link_libraries(stockReader_test PRIVATE doctest)
add_executable(meta_test meta_test.cpp)
target_link_libraries(meta_test PRIVATE doctest)
add_executable(json_test json_test.cpp)
target_link_libraries(json_test PRIVATE doctest)

add_test(NAME meta_test COMMAND meta_test)
add_test(NAME json_test COMMAND json_test)
add_test(NAME stockReader_test COMMAND stockReader_test)
add_test(NAME library_test COMMAND library_test)

