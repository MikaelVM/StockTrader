#include "meta.h"
#include "doctest.h"
#include <string>
#include <vector>
#include <map>

/** tests multiple predicates on the specified type T using specified correctness values */
template <typename T>
bool test(bool pointer, bool boolean, bool number, bool character, bool container, bool str)
{
    CHECK(std::is_pointer_v<T> == pointer);
    // CHECK(std::is_pointer_v<T&> == pointer); // std functions are not robust against refs
    CHECK(std::is_pointer_v<T const> == pointer);
    /** TODO: the relevant meta-predicate tests as they get implemented  */
    CHECK(is_bool_v<T> == boolean);
    CHECK(is_bool_v<T&> == boolean);
    CHECK(is_bool_v<T const> == boolean);
    CHECK(is_bool_v<T const&> == boolean);
    CHECK(is_number_v<T> == number);
    CHECK(is_number_v<T&> == number);
    CHECK(is_number_v<T const> == number);
    CHECK(is_number_v<T const&> == number);
    CHECK(is_character_v<T> == character);
    CHECK(is_character_v<T&> == character);
    CHECK(is_character_v<T const> == character);
    CHECK(is_character_v<T const&> == character);
    CHECK(is_container_v<T> == container);
    CHECK(is_container_v<T&> == container);
    CHECK(is_container_v<T const> == container);
    CHECK(is_container_v<T const&> == container);
    CHECK(is_string_v<T> == str);
    CHECK(is_string_v<T&> == str);
    CHECK(is_string_v<T const> == str);
    CHECK(is_string_v<T const&> == str);
    return true;
}
TEST_CASE("meta functions")
{
    auto cstring = "hello";
    int ai[] = {1, 2, 3, 4, 5};            // C++ array of integers
    auto pai = new int[5]{1, 2, 3, 4, 5};  // pointer to array of integers
    auto ili = {1, 2, 3, 4, 5};            // initializer list of integers
    auto ilc = {'a', 'b', 'c', 'd', 'e'};  // initializer list of characters
    SUBCASE("bool")
    {
        //         ptr    bool  number char   cont.  str.
        test<bool>(false, true, false, false, false, false);
    }
    SUBCASE("char")
    {
        //         ptr    bool   number char cont.  str.
        test<char>(false, false, true, true, false, false);
    }
    SUBCASE("int")
    {
        //        ptr    bool   number char  cont.  str.
        test<int>(false, false, true, false, false, false);
    }
    SUBCASE("char*")
    {
        //          ptr    bool  number char  cont.  str.
        test<char*>(true, false, false, false, false, true);
    }
    SUBCASE("cstring")
    {
        //                      ptr    bool   number char   cont.  str.
        test<decltype(cstring)>(true, false, false, false, false, true);
    }
    SUBCASE("std::string")
    {
        //                ptr    bool   number char   cont. str.
        test<std::string>(false, false, false, false, true, true);
    }
    SUBCASE("int")
    {
        //                     ptr    bool   number char   cont.  str.
        test<std::vector<int>>(false, false, false, false, true, false);
    }
    SUBCASE("array of int")
    {
        //                 ptr    bool   number char   cont.  str.
        test<decltype(ai)>(false, false, false, false, true, false);
    }
    SUBCASE("pointer to array of int")
    {
        //                  ptr    bool   number char   cont.  str.
        test<decltype(pai)>(true, false, false, false, false, false);
    }
    SUBCASE("initializer_list<int>")
    {
        //                  ptr    bool   number char   cont. str.
        test<decltype(ili)>(false, false, false, false, true, false);
    }
    SUBCASE("initializer_list<char>")
    {
        //                  ptr    bool   number char  cont.  str.
        test<decltype(ilc)>(false, false, false, false, true, true);
    }
    SUBCASE("std::map<int,int>")
    {
        //                       ptr    bool   number char   cont. str.
        test<std::map<int, int>>(false, false, false, false, true, false);
    }
    SUBCASE("tuple")
    {
        auto tids = std::tuple{7, 3.14, "hello"};
        CHECK(is_tuple_v<decltype(tids)> == true);
        CHECK(is_tuple_v<int> == false);
    }
    SUBCASE("Container Concept")
    {
        CHECK(Container<std::vector<int>> == true);
        CHECK(Container<int> == false);
        CHECK(Container<std::string> == false);
    }
    delete[] pai;
}
