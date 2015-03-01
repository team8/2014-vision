cmake_minimum_required(VERSION 2.8.4)
project(OpenCV_Vision)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Vision ${SOURCE_FILES})
target_link_libraries(Vision ${OpenCV_LIBS})

add_executable(Calibration calibration.cpp)
target_link_libraries(Calibration ${OpenCV_LIBS})

add_executable(TeleopGuide teleop_guide.cpp)
target_link_libraries(TeleopGuide ${OpenCV_LIBS})