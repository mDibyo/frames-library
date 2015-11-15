#ifndef FREENECT2_TEST_FRAMES_IO_H
#define FREENECT2_TEST_FRAMES_IO_H

#include <iostream>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>


int openDeviceAndAddListener(libfreenect2::SyncMultiFrameListener* listener);

int closeDevice();

#endif //FREENECT2_TEST_FRAMES_IO_H
