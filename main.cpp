#include <iostream>
#include <signal.h>
#include <time.h>

#include "frames_io.h"
#include "frames_transform.h"

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

void motion_unit(FramesInputter &inputter, FramesOutputter &outputter) {
  libfreenect2::FrameMap frames;
  libfreenect2::FrameMap derived_frames;

  // transformers
  FramesNewPairwiseDistanceTransformer distance_transformer;
  FramesInplaceMinThresholdTransformer threshold_transformer(30);
  FramesInplaceMaskTransformer mask_transformer(100);

  int frame_count = 0;
  while (!program_shutdown) {
    if (!inputter.getNextFrame(frames)) {
      std::cout << "no more frames" << std::endl;
      break;
    }

    clock_t start = clock();
    distance_transformer.transform(frames, derived_frames);

    if (frame_count > 0) {
      std::cout << "thresholding" << std::endl;
      threshold_transformer.transform(derived_frames);

      std::cout << "masking" << std::endl;
      mask_transformer.transform(frames, derived_frames);
    }

    std::cout << (double)(clock() - start) / CLOCKS_PER_SEC << std::endl;

    if (!outputter.putNextFrame(frames)) {
      std::cout << "could not write out frame" << std::endl;
    }

    frame_count++;
  }
}

int main(int argc, char *argv[]) {
  std::cout << "Starting up" << std::endl;
  signal(SIGINT, sigint_handler);
  program_shutdown = false;

  FramesInputterFromDisk inputter("pendulum/filebase_rgb_");
  FramesOutputterToDiskPNG outputter("pendulum_motion/rgb_");

  try {
    motion_unit(inputter, outputter);
  } catch (FramesIOException& exception) {
    std::cout << "Could not initialize input. " << std::endl;
    return -1;
  }

  return 0;
}

