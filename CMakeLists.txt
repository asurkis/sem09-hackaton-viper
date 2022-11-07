cmake_minimum_required(VERSION 3.0)
project(Viper)

find_package(raylib 4 REQUIRED)

add_executable(viper src/main.cpp)
target_link_libraries(viper raylib)
