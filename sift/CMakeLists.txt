cmake_minimum_required(VERSION 3.6)
project(opencv_test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
find_package(OpenCV REQUIRED)
add_executable(opencv_test ${SOURCE_FILES} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})