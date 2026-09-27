cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Rasterizer
    main.cpp
    FrameBuffer.cpp
    Geometry.cpp
    Rasterizer.cpp
    Vector3.cpp
    Matrix4.cpp
    Transform.cpp
    Clipper.cpp
)
