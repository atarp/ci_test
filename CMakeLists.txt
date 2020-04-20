cmake_minimum_required(VERSION 3.9)
project(MD5Encrypter)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_compile_options(-Wall -Wextra -Wpedantic)

#add_executable(intro md5.cpp)

add_executable(md5 md5.cpp)
target_link_libraries(md5 ${CONAN_LIBS})

#target_compile_features(intro PRIVATE cxx_lambda_init_captures)
#target_link_libraries(intro PRIVATE project_options project_warnings CONAN_PKG::docopt.cpp CONAN_PKG::fmt CONAN_PKG::spdlog)
