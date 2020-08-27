cmake_minimum_required(VERSION 2.8.10)
project(demo_github C CXX)

include_directories(
                    ${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/include/
                   )
link_directories(
                 ${PROJECT_SOURCE_DIR}/
                 ${PROJECT_SOURCE_DIR}/lib/
                 ${PROJECT_SOURCE_DIR}/lib/boost_1_56_0
                )

set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -O0 -W -Wall -Wno-unused-parameter -fPIC -std=c++11")

add_executable(demo_github demo_github.cpp)
#依赖libssl.so, libcrypto.so
target_link_libraries(demo_github libbaker-synthesizer-linux.so libssl.so libcrypto.so)


