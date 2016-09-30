cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(VIS_PRO1)

find_package(OpenCV REQUIRED)


add_executable(image1 src/image1.cpp)
target_link_libraries(image1 ${OpenCV_LIBS})

add_executable(image2 src/image2.cpp)
target_link_libraries(image2 ${OpenCV_LIBS})


add_executable(image3 src/image3.cpp)
target_link_libraries(image3 ${OpenCV_LIBS})


add_executable(image4_1 src/image4_1.cpp)
target_link_libraries(image4_1 ${OpenCV_LIBS})

add_executable(image4_2 src/image4_2.cpp)
target_link_libraries(image4_2 ${OpenCV_LIBS})

