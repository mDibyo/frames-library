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


FramesInputterFromDisk::FramesInputterFromDisk(
    std::string prefix,
    size_t width=DEFAULT_FRAME_WIDTH,
    size_t height=DEFAULT_FRAME_HEIGHT,
    size_t bytes_per_pixel=DEFAULT_FRAME_BYTES_PER_PIXEL)
    : input_prefix(prefix),
      current_frame_idx(-1),
      frame(new libfreenect2::Frame(width, height, bytes_per_pixel)) { }

FramesInputterFromDisk::~FramesInputterFromDisk() {
  delete frame;
}

bool FramesInputterFromDisk::getNextFrame(libfreenect2::FrameMap &frames) {
  current_frame_idx += 1;

  char filename[256];
  std::sprintf(filename, (input_prefix + "%d").c_str(), current_frame_idx);
  std::cout << filename << std::endl;

  std::ifstream file(filename, std::ios::binary|std::ios::ate);
  if (!file.is_open()) {
    std::cout << "file could not be opened for reading" << std::endl;
    return false;
  }

  int exp_filesize = frame->width * frame->height * frame->bytes_per_pixel;
  if (file.tellg() != exp_filesize) {
    std::cout << "file not of the correct size" << std::endl;
    return false;
  }

  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char *>(frame->data), exp_filesize);
  frames[libfreenect2::Frame::Color] = frame;

  file.close();
  return true;
}


FramesOutputterToDisk::FramesOutputterToDisk(std::string prefix)
    : output_prefix(prefix),
      current_frame_idx(-1) { }

bool FramesOutputterToDisk::putNextFrame(libfreenect2::FrameMap & frames) {
  if (frames.count(libfreenect2::Frame::Color) == 0) {
    std::cout << "color frame not present" << std::endl;
    return false;
  }

  current_frame_idx += 1;

  char filename[256];
  std::sprintf(filename, (output_prefix + "%d").c_str(), current_frame_idx);
  std::cout << filename << std::endl;

  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cout << "file could not be opened for writing" << std::endl;
    return false;
  }

  libfreenect2::Frame *frame = frames[libfreenect2::Frame::Color];
  int filesize = frame->width * frame->height * frame->bytes_per_pixel;
  file.write(reinterpret_cast<const char *>(frame->data), filesize);

  file.close();
  return true;
}