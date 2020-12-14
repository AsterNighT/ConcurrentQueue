cmake_minimum_required(VERSION 3.0.0)
project(conque VERSION 0.1.0)
add_executable(conque main.cpp test.cpp test.hpp queue.hpp)
set_property(TARGET conque PROPERTY CXX_STANDARD 17)
