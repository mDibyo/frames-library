#include <iostream>
#include <signal.h>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include "frames_io.h"


bool kinect_shutdown = false;

libfreenect2::Freenect2Device *device = 0;

void sigint_handler(int s) {
  kinect_shutdown = true;
}

libfreenect2::FrameListener* openDeviceAndGetListener(libfreenect2::Frame::Type frame_types) {
  libfreenect2::Freenect2 freenect2;

  if (freenect2.enumerateDevices() == 0) {
    std::cout << "no devices connected!" << std::endl;
    return NULL;
  }

  device = freenect2.openDefaultDevice();
  if (device == 0) {
    std::cout << "failure opening device!" << std::endl;
    return NULL;
  }

  signal(SIGINT, sigint_handler);
  kinect_shutdown = false;

  libfreenect2::SyncMultiFrameListener listener(frame_types);
  //  libfreenect2::Frame::Type::Color
  if ((int) frame_types % 2) {
    device->setColorFrameListener(&listener);
  }
  // libfreenect2::Frame::Type::Ir | libfreenect2::Frame::Type::Depth
  if (!((int) frame_types % 2)) {
    device->setIrAndDepthFrameListener(&listener);
  }

  device->start();
  return &listener;
}

int closeDevice() {
  if (device == 0) {
    std::cout << "device not open!" << std::endl;
  }

  device->stop();
  device->close();
  return 0;
}