cmake_minimum_required(VERSION 3.0)
project(Viper)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(viper src/main.cpp src/Context.hpp src/Handler.hpp src/CommandParser.hpp)
target_link_libraries(viper sfml-graphics)
