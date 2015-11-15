#ifndef FREENECT2_TEST_FRAMES_IO_H
#define FREENECT2_TEST_FRAMES_IO_H

#include <iostream>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>


libfreenect2::Freenect2Device *device = 0;

libfreenect2::SyncMultiFrameListener* openDeviceAndGetListener(libfreenect2::Frame::Type frame_types);

int closeDevice();

#endif //FREENECT2_TEST_FRAMES_IO_H
