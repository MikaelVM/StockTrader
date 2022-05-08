add_library(doctest OBJECT doctest.cpp)

add_executable(library_test library_test.cpp)
target_link_libraries(library_test PRIVATE doctest)
add_executable(stockReader_test stockReader_test.cpp)
target_link_libraries(stockReader_test PRIVATE doctest)

add_test(NAME stockReader_test COMMAND stockReader_test)
add_test(NAME library_test COMMAND library_test)

