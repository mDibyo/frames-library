#include "frames_io.h"

#include <fstream>


FramesInputterFromDevice::FramesInputterFromDevice()
    : listener(new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color)) {
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

FramesInputterFromDevice::~FramesInputterFromDevice() {
  if (device == 0) {
    return;
  }
  device->stop();
  device->close();
}

bool FramesInputterFromDevice::getNextFrame(libfreenect2::FrameMap &frames) {
  listener->waitForNewFrame(frames);
  return true;
}


FramesInputterFromDisk::FramesInputterFromDisk(std::string prefix)
    : frame(new libfreenect2::Frame(FRAME_WIDTH, FRAME_HEIGHT, FRAME_BYTES_PER_PIXEL)) {
  input_prefix = prefix;
  current_frame_idx = -1;
}

bool FramesInputterFromDisk::getNextFrame(libfreenect2::FrameMap &frames) {
  current_frame_idx += 1;

  char filename[256];
  std::sprintf(filename, (input_prefix + "%d").c_str(), current_frame_idx);
  std::cout << filename << std::endl;

  std::ifstream file(filename, std::ios::binary|std::ios::ate);
  if (!file.is_open())
    return false;

  int exp_filesize = FRAME_WIDTH * FRAME_HEIGHT * FRAME_BYTES_PER_PIXEL;
  if (file.tellg() != exp_filesize) {
    std::cout << "file not of the correct size" << std::endl;
    return false;
  }

  file.read(reinterpret_cast<char *>(frame->data), exp_filesize);
  frames[libfreenect2::Frame::Color] = frame;
  return true;
}