#include "frames_io.h"


void FramesInputterFromDevice::getNextFrame(libfreenect2::FrameMap frames) {
  listener->waitForNewFrame(frames);
}