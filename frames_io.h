#ifndef FREENECT2_TEST_FRAMES_IO_H
#define FREENECT2_TEST_FRAMES_IO_H

#include <iostream>
#include <stdexcept>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>


class FramesIOException : public std::runtime_error {
public:
  FramesIOException(std::string reason) : std::runtime_error(reason) { }
};


class FramesInputter {
public:
  virtual void getNextFrame(libfreenect2::FrameMap) { }
};


class FramesInputterFromDevice: public FramesInputter {
  libfreenect2::Freenect2 freenect2;
  libfreenect2::Freenect2Device *device;
  libfreenect2::SyncMultiFrameListener *listener;


public:

  FramesInputterFromDevice() :
      listener(new libfreenect2::SyncMultiFrameListener(
          libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth)) {
    if (freenect2.enumerateDevices() == 0) {
      std::cout << "no devices connected!" << std::endl;
      throw FramesIOException("no device connected!");
    }

    std::string serial = freenect2.getDefaultDeviceSerialNumber();
    device = freenect2.openDevice(serial);
    if (device == 0) {
      std::cout << "failure opening device!" << std::endl;
      throw FramesIOException("failure opening device!");
    }

    device->setColorFrameListener(listener);
    device->setIrAndDepthFrameListener(listener);

    device->start();
  }

  ~FramesInputterFromDevice() {
    if (device == 0) {
      return;
    }
    device->stop();
    device->close();
  }

  void getNextFrame(libfreenect2::FrameMap);
};


#endif //FREENECT2_TEST_FRAMES_IO_H
