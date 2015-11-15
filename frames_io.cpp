#include "frames_io.h"


libfreenect2::Freenect2Device *device = 0;

int openDeviceAndAddListener(libfreenect2::SyncMultiFrameListener* listener) {
  libfreenect2::Freenect2 freenect2;

  if (freenect2.enumerateDevices() == 0) {
    std::cout << "no devices connected!" << std::endl;
    return -1;
  }

  device = freenect2.openDefaultDevice();
  if (device == 0) {
    std::cout << "failure opening device!" << std::endl;
    return -1;
  }

  //  libfreenect2::Frame::Type::Color
  device->setColorFrameListener(listener);
  // libfreenect2::Frame::Type::Ir | libfreenect2::Frame::Type::Depth
  device->setIrAndDepthFrameListener(listener);

  device->start();
  return 0;
}

int closeDevice() {
  if (device == 0) {
    std::cout << "device not open!" << std::endl;
    return -1;
  }

  device->stop();
  device->close();
  return 0;
}