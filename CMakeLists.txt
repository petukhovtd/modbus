cmake_minimum_required(VERSION 3.0)
project(modbus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

option(MODBUS_UNIT_TEST "Enable unit-test" OFF)
option(MODBUS_EXAMPLE "Enable example" OFF)

add_subdirectory(third_party)
add_subdirectory(modbus)
if(MODBUS_EXAMPLE)
     add_subdirectory(example)
endif()