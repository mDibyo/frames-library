#include <iostream>
#include <signal.h>

#include "frames_io.h"

bool program_shutdown = false;

void sigint_handler(int s) {
  program_shutdown = true;
}

void identity_unit(FramesInputter &inputter, FramesOutputter &outputter) {
  libfreenect2::FrameMap frames;
  while (!program_shutdown) {
    if (!inputter.getNextFrame(frames)) {
      std::cout << "no more frames" << std::endl;
      break;
    }

    if (!outputter.putNextFrame(frames)) {
      std::cout << "could not write out frame" << std::endl;
    }
  }
}


int main(int argc, char *argv[]) {
  std::cout << "Starting up" << std::endl;
  signal(SIGINT, sigint_handler);
  program_shutdown = false;

  FramesInputterFromDisk inputter("pendulum/filebase_rgb_");
  FramesOutputterToDiskPNG outputter("nonpendulum/rgb_");

  try {
    identity_unit(inputter, outputter);
  } catch (FramesIOException& exception) {
    std::cout << "Could not initialize input. " << std::endl;
    return -1;
  }

  return 0;
}

