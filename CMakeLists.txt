cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE  Debug) # CACHE STRING "Choose the type of build." FORCE)

project(MD5Encrypter)

add_definitions("-std=c++11")

#add_library(project_options INTERFACE)
#target_compile_definitions(project_options INTERFACE cxx_std_20)

#########CONAN############################
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES}
                         poco/1.10.1)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  	file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.13/conan.cmake"
       		"${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_add_remote(NAME bincrafters URL
                 https://api.bintray.com/conan/bincrafters/public-conan)

#conan_cmake_run(
#  REQUIRES
#  ${CONAN_EXTRA_REQUIRES}
#  OPTIONS
#  ${CONAN_EXTRA_OPTIONS}
#  BASIC_SETUP
#  CMAKE_TARGETS # individual targets to link to
#  BUILD
#  missing)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
message( "Conan_Libs: ${CONAN_LIBS}")

###################################################################

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(md5 md5.cpp)

message( "Project_Options: ${project_options}")
message( "Project_Warnings: ${project_warnings}")

target_link_libraries(md5 ${CONAN_LIBS})

target_compile_features(md5 PRIVATE cxx_lambda_init_captures)
