project(example1)

add_executable(example1 main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)

target_link_libraries(example1 PRIVATE Poco::Foundation Poco::Net)
