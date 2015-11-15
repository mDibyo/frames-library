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

  try {
    FramesInputterFromDevice inputter;

    libfreenect2::FrameMap frames;
    while (!kinect_shutdown) {
      inputter.getNextFrame(frames);

      // deal with frames
      std::cout << "Dealing with frames" << std::endl;
    }
  } catch (FramesIOException& exception) {
    std::cout << "Could not initialize input. " << std::endl;
    return -1;
  }

  return 0;
}

