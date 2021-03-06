cmake_minimum_required(VERSION 2.8.12)
project(frames_library)
set(CMAKE_BUILD_TYPE Debug)


link_directories(/usr/local/lib)
set(freenect2_LIBRARIES libfreenect2.so)

set(Protonect_LIBRARIES ${freenect2_LIBRARIES})

find_package(PNG)
if(PNG_FOUND)
  include_directories(${PNG_INCLUDE_DIRS})
  list(APPEND Protonect_LIBRARIES ${PNG_LIBRARIES})
  add_definitions(-DSAVE_WITH_LIBPNG=1)
endif()


add_library(Frames_LIBRARY
        frames_io.h
        frames_io.cpp
        frames_transform.h
        frames_transform.cpp)
target_link_libraries(Frames_LIBRARY ${Protonect_LIBRARIES})
target_include_directories(Frames_LIBRARY PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Recorder main.cpp)
target_link_libraries(Recorder ${Protonect_LIBRARIES} Frames_LIBRARY)
