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

# Executable 3: ArUco Marker Detection
add_executable(aruco_detection src/aruco_detection.cpp)
target_link_libraries(aruco_detection ${OpenCV_LIBS})

# Executable 4: Autonomous ArUco Navigation
add_executable(autonomous_aruco src/autonomous_aruco.cpp)
target_link_libraries(autonomous_aruco ${OpenCV_LIBS})

# Executable 5: Autonomous Rover (Camera -> Robot Bridge)
add_executable(autonomous_rover
    src/autonomous_rover.cpp
    src/robot_bridge.cpp
    src/serial_transport.cpp
)
target_link_libraries(autonomous_rover ${OpenCV_LIBS})
