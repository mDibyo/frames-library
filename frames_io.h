#ifndef FREENECT2_TEST_FRAMES_IO_H
#define FREENECT2_TEST_FRAMES_IO_H

#include <iostream>
#include <stdexcept>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>


const size_t DEFAULT_FRAME_WIDTH = 1920,
    DEFAULT_FRAME_HEIGHT = 1080,
    DEFAULT_FRAME_BYTES_PER_PIXEL = 4;


/**
 * Exception class thrown by constructors of frames inputter and outputter
 * classes.
 */
class FramesIOException : public std::runtime_error {
public:
  FramesIOException(std::string reason) : std::runtime_error(reason) { }
};


/**
 * Interface for classes that read frames into the system.
 */
class FramesInputter {
public:
  /**
   * Read in the next set of frames from the input source.
   */
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
  FramesInputterFromDisk(std::string, size_t=DEFAULT_FRAME_WIDTH,
                         size_t=DEFAULT_FRAME_HEIGHT,
                         size_t=DEFAULT_FRAME_BYTES_PER_PIXEL);

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
  /**
   * Write out the next set of frames to the output source.
   */
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
