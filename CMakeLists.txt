cmake_minimum_required(VERSION 3.11)

project(globalProject)

add_subdirectory(example1)
add_subdirectory(example2)

# find_package(fmt CONFIG REQUIRED)
# find_package(Poco REQUIRED COMPONENTS Foundation Net)

# add_executable(Hellov main.cpp)

# target_link_libraries(Hellov PRIVATE fmt::fmt)

# target_link_libraries(Hellov PRIVATE Poco::Foundation Poco::Net)
