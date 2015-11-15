#include <iostream>
#include <signal.h>

#include "frames_io.h"

bool kinect_shutdown = false;

void sigint_handler(int s) {
  kinect_shutdown = true;
}

int main(int argc, char *argv[]) {
  std::cout << "Starting up" << std::endl;
  signal(SIGINT, sigint_handler);
  kinect_shutdown = false;

  libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
  if (openDeviceAndAddListener(&listener) != 0) {
    std::cout << "Device not opening" << std::endl;
    return -1;
  }

  libfreenect2::FrameMap frames;

  while (!kinect_shutdown) {
    listener.waitForNewFrame(frames);

    // deal with frames
    std::cout << "Dealing with frames" << std::endl;
  }

  closeDevice();
  return 0;
}

