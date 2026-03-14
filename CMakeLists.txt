cmake_minimum_required(VERSION 3.14)
project(Qwen3_5VisionCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Python and Pybind11
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Fetch pybind11
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

add_executable(visionapp src/main.cpp src/vision_app.cpp src/utils.cpp)
target_include_directories(visionapp PRIVATE include)

if(WIN32)
    target_link_libraries(visionapp PRIVATE pybind11::embed ws2_32 crypt32)
else()
    target_link_libraries(visionapp PRIVATE pybind11::embed pthread)
endif()
