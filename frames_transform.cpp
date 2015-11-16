#include "frames_transform.h"

#include <string.h>

#include "frames_io.h"


FramesPairwiseAbsDiffTransformer::FramesPairwiseAbsDiffTransformer(
    size_t width=DEFAULT_FRAME_WIDTH,
    size_t height=DEFAULT_FRAME_HEIGHT,
    size_t bytes_per_pixel=DEFAULT_FRAME_BYTES_PER_PIXEL)
    : has_prev(false),
      prev_frame(new libfreenect2::Frame(width, height, bytes_per_pixel)) { }

FramesPairwiseAbsDiffTransformer::~FramesPairwiseAbsDiffTransformer() {
  delete prev_frame;
}

bool FramesPairwiseAbsDiffTransformer::transform(libfreenect2::FrameMap &frames) {
  size_t size = prev_frame->width * prev_frame->height * prev_frame->bytes_per_pixel;
  unsigned char *new_frame_data = frames[libfreenect2::Frame::Color]->data;
  unsigned char new_frame_data_copy[size];
  memcpy(new_frame_data_copy, new_frame_data, size);

  if (has_prev) {
    int diff = 0;
    for (size_t i = 0; i < size; i++) {
      diff = new_frame_data[i] - prev_frame->data[i];
      if (diff < 0) {
        new_frame_data[i] = (unsigned char) (- diff);
      } else {
        new_frame_data[i] = (unsigned char) diff;
      }
    }
  }

  prev_frame->data = new_frame_data;
  has_prev = true;
  return true;
}