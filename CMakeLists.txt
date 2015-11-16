CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(freenect2_test)
SET(CMAKE_BUILD_TYPE Debug)


LINK_DIRECTORIES(/usr/local/lib)
SET(freenect2_LIBRARIES libfreenect2.so)

SET(Protonect_LIBRARIES ${freenect2_LIBRARIES})

FIND_PACKAGE(PNG)
IF(PNG_FOUND)
  INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
  LIST(APPEND Protonect_LIBRARIES ${PNG_LIBRARIES})
  ADD_DEFINITIONS(-DSAVE_WITH_LIBPNG=1)
ENDIF()


ADD_LIBRARY(frame_library
        frames_io.h
        frames_io.cpp
        frames_transform.h
        frames_transform.cpp)
TARGET_LINK_LIBRARIES(frame_library ${Protonect_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(frame_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(Recorder main.cpp)
TARGET_LINK_LIBRARIES(Recorder ${Protonect_LIBRARIES} frame_library)
