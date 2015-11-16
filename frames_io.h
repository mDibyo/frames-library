#ifndef FREENECT2_TEST_FRAMES_IO_H
#define FREENECT2_TEST_FRAMES_IO_H

#include <iostream>
#include <stdexcept>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>


const size_t FRAME_WIDTH = 1920,
    FRAME_HEIGHT = 1080,
    FRAME_BYTES_PER_PIXEL = 4;


class FramesIOException : public std::runtime_error {
public:
  FramesIOException(std::string reason) : std::runtime_error(reason) { }
};


class FramesInputter {
public:
  virtual bool getNextFrame(libfreenect2::FrameMap &) = 0;
};


class FramesInputterFromDevice : public FramesInputter {
public:
  FramesInputterFromDevice();
  ~FramesInputterFromDevice();
  bool getNextFrame(libfreenect2::FrameMap &);

private:
  libfreenect2::Freenect2 freenect2;
  libfreenect2::Freenect2Device *device;
  libfreenect2::SyncMultiFrameListener *listener;
};


class FramesInputterFromDisk : public FramesInputter {
public:
  FramesInputterFromDisk(std::string prefix);
  bool getNextFrame(libfreenect2::FrameMap &);

private:
  std::string input_prefix;
  int current_frame_idx;
  libfreenect2::Frame *frame;
};


#endif //FREENECT2_TEST_FRAMES_IO_H
