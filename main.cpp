#include <iostream>
#include <signal.h>

#include "frames_io.h"

bool program_shutdown = false;

void sigint_handler(int s) {
  program_shutdown = true;
}

int main(int argc, char *argv[]) {
  std::cout << "Starting up" << std::endl;
  signal(SIGINT, sigint_handler);
  program_shutdown = false;

  try {
    FramesInputterFromDisk inputter("pendulum/filebase_rgb_");
    FramesOutputterToDiskPNG outputter("nonpendulum/rgb_");

    libfreenect2::FrameMap frames;
    std::cout << frames.count(libfreenect2::Frame::Color) << std::endl;
    while (!program_shutdown) {
      if (!inputter.getNextFrame(frames)) {
        std::cout << "no more frames" << std::endl;
        break;
      }

      if (!outputter.putNextFrame(frames)) {
        std::cout << "could not write out frame" << std::endl;
      }
    }
  } catch (FramesIOException& exception) {
    std::cout << "Could not initialize input. " << std::endl;
    return -1;
  }

  return 0;
}

