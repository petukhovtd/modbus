project(example)

add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE modbus)