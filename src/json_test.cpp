#include "json.h"
#include "meta.h"
#include "doctest.h"
#include <sstream>
#include <vector>

/** custom class to test JSON input/output */
struct aggregate_t
{ /** public access is just for easy structured initialization in tests */
    bool b;
    int x;
    double y;
    std::string z;
    std::vector<int> w;
    /** visitor support with read-only access, e.g. for writing-out */
    template <typename Visitor>
    void accept_reader(Visitor&& visit) const
    {
        visit("b", b);
        visit("x", x);
        visit("y", y);
        visit("z", z);
        visit("w", w);
    }
    /** visitor support with full access, e.g. for reading-in */
    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        visit("b", b);
        visit("x", x);
        visit("y", y);
        visit("z", z);
        visit("w", w);
    }
    /** equality operator to support testing */
    friend bool operator==(const aggregate_t& a1, const aggregate_t& a2)
    {
        return (a1.b == a2.b) && (a1.x == a2.x) && (a1.y == a2.y) && (a1.z == a2.z) && (a1.w == a2.w);
    }
};

/** custom class to test JSON input/output with nesting */
struct nested_t
{
    std::string text;
    aggregate_t agg;
    /** visitor support with read-only access, e.g. for writing-out */
    template <typename Visitor>
    void accept_reader(Visitor&& visit) const
    {
        visit("text", text);
        visit("agg", agg);
    }
    /** visitor support with full access, e.g. for reading-in */
    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        visit("text", text);
        visit("agg", agg);
    }
    /** equality operator to support testing */
    friend bool operator==(const nested_t& n1, const nested_t& n2)
    {
        return (n1.text == n2.text) && (n1.agg == n2.agg);
    }
};

struct unsupported
{
    int i, j;
};

using namespace std::string_literals;

/** Output operator just for friendly output in tests: */
std::ostream& operator<<(std::ostream& os, const aggregate_t& agg) { return os << json_t{agg}; }

/** Output operator just for friendly output in tests: */
std::ostream& operator<<(std::ostream& os, const nested_t& nested) { return os << json_t{nested}; }

TEST_CASE("JSON output")
{
    auto os = std::ostringstream{};
    SUBCASE("boolean: lvalue true")
    {
        auto v = true;
        os << json_t{v};
        CHECK(os.str() == "true");
    }
    SUBCASE("boolean: lvalue false")
    {
        auto v = false;
        os << json_t{v};
        CHECK(os.str() == "false");
    }
    SUBCASE("boolean: rvalue")
    {
        os << json_t{true};
        CHECK(os.str() == "true");
    }
    SUBCASE("integer")
    {
        os << json_t{7};
        CHECK(os.str() == "7");
    }
    SUBCASE("double")
    {
        os << json_t{3.14};
        CHECK(os.str() == "3.14");
    }
    SUBCASE("c-string")
    {
        os << json_t{"hello"};
        CHECK(os.str() == "\"hello\"");
    }
    SUBCASE("cpp-string")
    {
        os << json_t{"hello"s};
        CHECK(os.str() == "\"hello\"");
    }
    SUBCASE("container: lvalue")
    {
        auto v = std::vector{3, 7, 11};
        os << json_t{v};
        CHECK(os.str() == "[3,7,11]");
    }
    SUBCASE("container: rvalue")
    {
        os << json_t{std::vector{3, 7, 11}};
        CHECK(os.str() == "[3,7,11]");
    }
    SUBCASE("tuple")
    {
        auto v = std::tuple{7, 3.14, "hello"};
        os << json_t{v};
        CHECK(os.str() == R"({"1":7,"2":3.14,"3":"hello"})");
    }
    SUBCASE("aggregate")
    {
        auto v = aggregate_t{true, 3, 3.14, "hello", {7, 11}};
        static_assert(accepts_reader_v<aggregate_t, json_reader_t>, "does not accept reader");
        os << json_t{v};
        CHECK(os.str() == R"({"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]})");
    }
    SUBCASE("nested")
    {
        auto v = nested_t{"complicated", {true, 3, 3.14, "hello", {7, 11}}};
        static_assert(accepts_reader_v<nested_t, json_reader_t>, "does not accept reader");
        os << json_t{v};
        CHECK(os.str() == R"({"text":"complicated","agg":{"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]}})");
    }
    /* // uncomment to try unsupported type:
    SUBCASE("unsupported types"){
        auto fail = unsupported{3,4};
        os << json_t{fail};
    }
    */
}

TEST_CASE("JSON input")
{
    SUBCASE("boolean: true")
    {
        auto is = std::istringstream{"true"};
        auto v = false;
        is >> json_t{v};
        CHECK(is);
        CHECK(v == true);
    }
    SUBCASE("boolean: false")
    {
        auto is = std::istringstream{"false"};
        auto v = true;
        is >> json_t{v};
        CHECK(is);
        CHECK(v == false);
    }
    SUBCASE("integer")
    {
        auto is = std::istringstream{"7"};
        auto v = 0;
        is >> json_t{v};
        CHECK(is);
        CHECK(v == 7);
    }
    SUBCASE("double")
    {
        auto is = std::istringstream{"3.14"};
        auto v = 0.0;
        is >> json_t{v};
        CHECK(is);
        CHECK(v == 3.14);
    }
    SUBCASE("cpp-string")
    {
        auto is = std::istringstream{"\"hello\""};
        auto v = std::string{};
        is >> json_t{v};
        CHECK(is);
        CHECK(v == "hello");
    }
    SUBCASE("container")
    {
        auto is = std::istringstream{"[3,7,11]"};
        auto v = std::vector<int>{};
        is >> json_t{v};
        CHECK(is);
        CHECK(v == std::vector{3, 7, 11});
    }
    SUBCASE("tuple")
    {
        auto is = std::istringstream{R"({"1":7,"2":3.14,"3":"hello"})"};
        auto v = std::tuple{13, 2.71, "bye"s};
        is >> json_t{v};
        CHECK(v == std::tuple{7, 3.14, "hello"});
    }
    SUBCASE("aggregate")
    {
        auto is = std::istringstream{R"({"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]})"};
        auto v = aggregate_t{};
        static_assert(accepts_writer_v<aggregate_t, json_writer_t>, "does not accept writer");
        is >> json_t{v};
        CHECK(is);
        CHECK(v == aggregate_t{true, 3, 3.14, "hello", {7, 11}});
    }
    SUBCASE("nested")
    {
        auto is =
            std::istringstream{R"({"text":"complicated","agg":{"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]}})"};
        auto v = nested_t{};
        static_assert(accepts_writer_v<nested_t, json_writer_t>, "does not accept writer");
        is >> json_t{v};
        CHECK(is);
        CHECK(v == nested_t{"complicated", {true, 3, 3.14, "hello", {7, 11}}});
    }
    /* // uncomment to try unsupported type:
    SUBCASE("unsupported types"){
        auto fail = unsupported{3,4};
        os << json_t{fail};
    }
    */
}
