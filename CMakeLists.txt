cmake_minimum_required(VERSION 3.21)
project(StockTrader)

set(CMAKE_CXX_STANDARD 20)

add_library(StockTrader src/library_test.cpp include/stockReader.h include/json.h include/meta.h)

include_directories(include)

enable_testing()

add_subdirectory(src)