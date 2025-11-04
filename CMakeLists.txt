cmake_minimum_required(VERSION 3.0)

project( cobaAja )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

# Executable 1: Deteksi objek hitam
add_executable(cobaAja_kj src/cobaa.cpp)
target_link_libraries(cobaAja_kj ${OpenCV_LIBS})

# Executable 2: Face blur + Black object detection
add_executable(face_blur_detection src/face_blur_detection.cpp)
target_link_libraries(face_blur_detection ${OpenCV_LIBS})
