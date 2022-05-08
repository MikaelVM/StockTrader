//
// Created by mikba on 08/05/2022.
//

#ifndef STOCKTRADER_STOCKREADER_H
#define STOCKTRADER_STOCKREADER_H

#include <iostream>
//#include <iomanip>

template <typename C, typename = void>
struct is_container : std::false_type
{};

template <typename T>
struct json_t
{
    T&& data;
};

template <typename C>
constexpr auto is_container_v = is_container<C>::value;

char peek_non_ws(std::istream& is)
{
    char c;
    while (is && std::isspace(is.peek()))
        is.get(c);
    return static_cast<char>(is.peek());
}

char read_non_ws(std::istream& is)
{
    char c;
    while (is.get(c) && std::isspace(c))
        ;
    return c;
}

void expect(std::istream& is, const char symbol)
{
    using namespace std::literals::string_literals;
    const auto c = read_non_ws(is);
    if (symbol != c)
        throw std::logic_error("Expected "s + symbol + " but got '" + c + "'");
}

template <typename T>
std::istream& operator>>(std::istream& is, json_t<T>& json){
    if constexpr (is_container_v<T>) {
        // here support only STD containers, as arrays cannot change size
        // alternatively one may implement filling of the pre-allocated container
        using E = typename std::remove_reference_t<T>::value_type;
        expect(is, '[');
        auto first = true;
        while (is) {
            auto c = peek_non_ws(is);
            if (c == ']') {
                is.get(c);
                break;
            }
            if (!first) {
                if (c != ',')
                    throw std::logic_error("Expected ',' but got "s + c);
                is.get(c);
            } else
                first = false;
            E value{};
            is >> json_t{value};
            json.data.push_back(std::move(value));
        }
}
    else {throw std::logic_error("ERROR: Not JSON file");}
}


#endif //STOCKTRADER_STOCKREADER_H