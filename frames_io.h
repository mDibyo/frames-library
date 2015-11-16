#ifndef FREENECT2_TEST_FRAMES_IO_H
#define FREENECT2_TEST_FRAMES_IO_H

#include <iostream>
#include <stdexcept>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>


const size_t FRAME_WIDTH = 1920,
    FRAME_HEIGHT = 1080,
    FRAME_BYTES_PER_PIXEL = 4;


/**
 * Interface for classes that read frames into the system.
 */
class FramesIOException : public std::runtime_error {
public:
  FramesIOException(std::string reason) : std::runtime_error(reason) { }
};


class FramesInputter {
public:
  virtual bool getNextFrame(libfreenect2::FrameMap &) = 0;
};


/**
 * Inputter class from reading in frames from device.
 */
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


/**
 * Inputter class for reading in frames from disk.
 */
class FramesInputterFromDisk : public FramesInputter {
public:
  FramesInputterFromDisk(std::string);
  ~FramesInputterFromDisk();
  bool getNextFrame(libfreenect2::FrameMap &);

private:
  std::string input_prefix;
  int current_frame_idx;
  libfreenect2::Frame *frame;
};


/**
 * Interface for classes that write frames out of the system.
 */
class FramesOutputter {
public:
  virtual bool putNextFrame(libfreenect2::FrameMap &) = 0;
};


/**
 * Outputter class for writing out frames to disk as a byte array.
 */
class FramesOutputterToDisk : public FramesOutputter {
public:
  FramesOutputterToDisk(std::string);
  bool putNextFrame(libfreenect2::FrameMap &);

private:
  std::string output_prefix;
  int current_frame_idx;
};


#endif //FREENECT2_TEST_FRAMES_IO_H
