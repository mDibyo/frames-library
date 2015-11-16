#ifndef FREENECT2_TEST_FRAMES_TRANSFORM_H
#define FREENECT2_TEST_FRAMES_TRANSFORM_H

#include <libfreenect2/frame_listener_impl.h>

#include "frames_io.h"


/**
 * Interface for classes that transform frames in place.
 */
class FramesTransformer {
public:
  /**
   * Transform the set of frames in place.
   */
  virtual bool transform(libfreenect2::FrameMap &frames) = 0;
};


/**
 * Class that performs the identity transform.
 */
class FramesIdentityTransformer {
public:
  static bool transform(libfreenect2::FrameMap & frames) { }
};


/**
 * Class that takes in pairs of consecutive frames and returns their pixel-wise
 * difference. The prior is considered to be a zero image and therefore the
 * first frame is not transformed.
 */
class FramesPairwiseAbsDiffTransformer {
public:
  FramesPairwiseAbsDiffTransformer(size_t=DEFAULT_FRAME_WIDTH,
                                   size_t=DEFAULT_FRAME_HEIGHT,
                                   size_t=DEFAULT_FRAME_BYTES_PER_PIXEL);

  ~FramesPairwiseAbsDiffTransformer();

  bool transform(libfreenect2::FrameMap &);

private:
  bool has_prev;
  libfreenect2::Frame *prev_frame;
};


/**
 * Class that zeros out all pixel values in a frame that are below a minimum
 * threshold value.
 */
class FramesMinThresholdTransformer {
public:
  FramesMinThresholdTransformer(unsigned char);

  bool transform(libfreenect2::FrameMap &);

private:
  unsigned char min_threshold;
};

#endif //FREENECT2_TEST_FRAMES_TRANSFORM_H
