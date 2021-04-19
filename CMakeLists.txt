# minimum cmake version
cmake_minimum_required(VERSION 3.10)

# project name, version
project(DDS_example VERSION 0.9.0)

execute_process(COMMAND git submodule update --init)
add_subdirectory(external/cyclonedds)

#add_subdirectory(simple)
