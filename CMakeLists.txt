cmake_minimum_required(VERSION 3.10)

if (DEFINED ENV{BUILD_NUMBER})
  add_definitions(-DBUILD_NUMBER=$ENV{BUILD_NUMBER})
endif()

include_directories(${PROJECT_SOURCE_DIR})
add_executable(helloworld main.cpp)

target_compile_features(helloworld PUBLIC cxx_std_17)