cmake_minimum_required(VERSION 3.0)
project(modbus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

option(MODBUS_TEST "Enable tests" OFF)
option(MODBUS_EXAMPLE "Enable example" OFF)

add_subdirectory(contrib)
if(MODBUS_EXAMPLE)
     add_subdirectory(example)
endif()
if(MODBUS_TEST)
     add_subdirectory(test)
endif()

set(MODBUS_SOURCE
        src/modbus_types.cpp
        src/modbus_framing.cpp
        src/modbus_buffer.cpp
        src/modbus_buffer_tcp_wrapper.cpp
        src/modbus_buffer_ascii_wrapper.cpp
        src/modbus_buffer_rtu_wrapper.cpp
        src/modbus_buffer_wrapper.cpp
)

add_library(${PROJECT_NAME} ${MODBUS_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Werror
        -Wextra
        -Wpedantic
        -Weffc++
        -Wold-style-cast
        -Wctor-dtor-privacy
        -Woverloaded-virtual
        -Wno-sign-conversion
        -Wno-unused-parameter
        -Wno-uninitialized
)
