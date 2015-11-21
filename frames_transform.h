#ifndef FREENECT2_TEST_FRAMES_TRANSFORM_H
#define FREENECT2_TEST_FRAMES_TRANSFORM_H

#include <libfreenect2/frame_listener_impl.h>

#include "frames_io.h"


/**
 * Interface for classes that transform frames in place.
 */
class FramesInplaceTransformer {
public:
  /**
   * Transform the set of frames in place.
   */
  virtual bool transform(libfreenect2::FrameMap &frames) = 0;
  virtual bool transform(libfreenect2::FrameMap &input_frames, libfreenect2::FrameMap &added_frames) = 0;
};


/**
 * Class that performs the identity transform.
 */
class FramesInplaceIdentityTransformer : public FramesInplaceTransformer {
public:
  bool transform(libfreenect2::FrameMap & frames) { }
};


/**
 * Class that takes in pairs of consecutive frames and returns their pixel-wise
 * difference. The prior is considered to be a zero image and therefore the
 * first frame is not transformed.
 */
class FramesInplacePairwiseAbsDiffTransformer : public FramesInplaceTransformer {
public:
  FramesInplacePairwiseAbsDiffTransformer(size_t=DEFAULT_FRAME_WIDTH,
                                          size_t=DEFAULT_FRAME_HEIGHT,
                                          size_t=DEFAULT_FRAME_BYTES_PER_PIXEL);

  ~FramesInplacePairwiseAbsDiffTransformer();

  bool transform(libfreenect2::FrameMap &);

private:
  bool has_prev;
  libfreenect2::Frame *prev_frame;
};


/**
 * Class that zeros out all pixel values in a frame that are below a minimum
 * threshold value.
 */
class FramesInplaceMinThresholdTransformer : public FramesInplaceTransformer {
public:
  FramesInplaceMinThresholdTransformer(unsigned char);

  bool transform(libfreenect2::FrameMap &);

private:
  unsigned char min_threshold;
};


/**
 * []
 */
class FramesInplaceMaskTransformer : public FramesInplaceTransformer {
public:
  FramesInplaceMaskTransformer(int);

  bool transform(libfreenect2::FrameMap &, libfreenect2::FrameMap &);

private:
  int maskAmount;
};



/**
 * Interface for classes that transform frames and create new ones in the process.
 */
class FramesNewTransformer {
public:
  /**
   * Transform the set of input frames and add results to the output set.
   */
  virtual bool transform(libfreenect2::FrameMap &inputFrames,
                         libfreenect2::FrameMap &outputFrames) = 0;
};


/**
 * Class that takes in pairs of consecutive frames and returns their pixel-wise
 * euclidean distance. The prior is considered to be the first frame and therefore
 * the first frame is not transformed.
 */
class FramesNewPairwiseDistanceTransformer : public FramesNewTransformer {
public:
  FramesNewPairwiseDistanceTransformer(size_t=DEFAULT_FRAME_WIDTH,
                                       size_t=DEFAULT_FRAME_HEIGHT,
                                       size_t=DEFAULT_FRAME_BYTES_PER_PIXEL);

  ~FramesNewPairwiseDistanceTransformer();

  bool transform(libfreenect2::FrameMap &, libfreenect2::FrameMap &);

private:
  bool has_prev;
  libfreenect2::Frame *prev_frame;
};

#endif //FREENECT2_TEST_FRAMES_TRANSFORM_H
