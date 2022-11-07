cmake_minimum_required(VERSION 3.0)
project(Viper)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(viper src/main.cpp)
target_link_libraries(viper sfml-graphics)
