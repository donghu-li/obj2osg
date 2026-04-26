cmake_minimum_required(VERSION 3.10)
project(OSGModelConverter)

set(CMAKE_CXX_STANDARD 14)

# 寻找 OSG 库
find_package(OpenSceneGraph REQUIRED osg osgDB osgUtil)

add_executable(osg_converter converter.cpp)

target_include_directories(osg_converter PRIVATE ${OPENSCENEGRAPH_INCLUDE_DIRS})
target_link_libraries(osg_converter PRIVATE ${OPENSCENEGRAPH_LIBRARIES})
