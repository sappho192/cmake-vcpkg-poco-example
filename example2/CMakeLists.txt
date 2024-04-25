project(example1)

add_executable(example2 main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)

target_link_libraries(example2 PRIVATE Poco::Foundation Poco::Net)
