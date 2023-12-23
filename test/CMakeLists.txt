project(test)

set(${PROJECT_NAME}_SOURCE
          modbus_common.cpp

          test_modbus_buffer.cpp
          test_modbus_buffer_ascii_wrapper.cpp
          test_modbus_buffer_rtu_wrapper.cpp
          test_modbus_buffer_tcp_wrapper.cpp
          test_modbus_framing.cpp
          test_modbus_types.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME}
          modbus
          gtest
          gtest_main
)
