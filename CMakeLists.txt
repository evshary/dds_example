# minimum cmake version
cmake_minimum_required(VERSION 3.10)

# project name, version
project(DDS_example VERSION 0.9.0)

execute_process(COMMAND git submodule update --init)
add_subdirectory(external/cyclonedds)

add_subdirectory(simple_example)
add_subdirectory(status_example)
add_subdirectory(instance_example)
add_subdirectory(listener_example)
add_subdirectory(ros_example)
