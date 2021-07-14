cmake_minimum_required(VERSION 2.8.3)
project(serial_example)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(
  include

)

add_executable(example src/main.cpp src/lx16a-servo.cpp src/serialib.cpp)


