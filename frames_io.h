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
  virtual bool getNextFrame(libfreenect2::FrameMap) { }
};


class FramesInputterFromDevice : public FramesInputter {
  libfreenect2::Freenect2 freenect2;
  libfreenect2::Freenect2Device *device;
  libfreenect2::SyncMultiFrameListener *listener;

public:
  FramesInputterFromDevice();
  ~FramesInputterFromDevice();
  bool getNextFrame(libfreenect2::FrameMap);
};


class FramesInputterFromDisk : public FramesInputter {
  std::string input_prefix;
  int current_frame_idx;

public:
  FramesInputterFromDisk(std::string prefix);
  bool getNextFrame(libfreenect2::FrameMap);
};


#endif //FREENECT2_TEST_FRAMES_IO_H
