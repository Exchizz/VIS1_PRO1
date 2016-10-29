cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(VIS_PRO1)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)
find_package(OpenCV REQUIRED)


add_executable(image1 src/image1.cpp src/functions.cpp)
target_link_libraries(image1 ${OpenCV_LIBS})

add_executable(image2 src/image2.cpp src/functions.cpp)
target_link_libraries(image2 ${OpenCV_LIBS})


add_executable(image3 src/image3.cpp src/functions.cpp)
target_link_libraries(image3 ${OpenCV_LIBS})


add_executable(image4_1 src/image4_1.cpp src/functions.cpp)
target_link_libraries(image4_1 ${OpenCV_LIBS})

add_executable(image4_2 src/image4_2.cpp src/functions.cpp)
target_link_libraries(image4_2 ${OpenCV_LIBS})

