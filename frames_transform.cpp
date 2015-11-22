#include "frames_transform.h"

#include <string.h>
#include <math.h>
#include <typeinfo>


FramesInplacePairwiseAbsDiffTransformer::FramesInplacePairwiseAbsDiffTransformer(
    size_t width, size_t height, size_t bytes_per_pixel)
    : has_prev(false),
      prev_frame(new libfreenect2::Frame(width, height, bytes_per_pixel)) { }

FramesInplacePairwiseAbsDiffTransformer::~FramesInplacePairwiseAbsDiffTransformer() {
  delete prev_frame;
}

bool FramesInplacePairwiseAbsDiffTransformer::transform(libfreenect2::FrameMap &frames) {
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


FramesInplaceMinThresholdTransformer::FramesInplaceMinThresholdTransformer(unsigned char threshold)
    : min_threshold(threshold) { }

bool FramesInplaceMinThresholdTransformer::transform(libfreenect2::FrameMap &frames) {
  libfreenect2::Frame *frame = frames[libfreenect2::Frame::Color];
  unsigned char *frame_data = frame->data;

  for (size_t i = 0; i < frame->width * frame->height * frame->bytes_per_pixel; i++) {
    if (frame_data[i] < min_threshold) {
      frame_data[i] = 0;
    }
  }

  return true;
}


FramesInplaceMaskTransformer::FramesInplaceMaskTransformer(int maskAmount)
    : maskAmount(maskAmount) { }


bool FramesInplaceMaskTransformer::transform(libfreenect2::FrameMap &input_frames,
                                             libfreenect2::FrameMap &added_frames) {
  libfreenect2::Frame *input_frame = input_frames[libfreenect2::Frame::Color];
  unsigned char *input_frame_data = input_frame->data;
  unsigned char *added_frame_data = added_frames[libfreenect2::Frame::Color]->data;

  int j, new_value;
  for (size_t i = 0; i < input_frame->width * input_frame->height; i++) {
    if (added_frame_data[i] == 0) {
      for (j = 0; j < input_frame->bytes_per_pixel; j++) {
        if (j == 3) {
          new_value = input_frame_data[i * input_frame->bytes_per_pixel + j] - maskAmount;
          if (new_value >= 0) {
            input_frame_data[i * input_frame->bytes_per_pixel + j] = new_value;
          } else {
            input_frame_data[i * input_frame->bytes_per_pixel + j] = 0;
          }
        }
      }
    }
  }
  return true;
}


FramesNewPairwiseDistanceTransformer::FramesNewPairwiseDistanceTransformer(
    size_t width, size_t height, size_t bytes_per_pixel)
    : has_prev(false),
      prev_frame(new libfreenect2::Frame(width, height, bytes_per_pixel)) {
  unsigned char *frame_data;
  frame_data = new unsigned char[width * height * bytes_per_pixel];
  prev_frame->data = frame_data;
}

FramesNewPairwiseDistanceTransformer::~FramesNewPairwiseDistanceTransformer() {
  delete prev_frame->data;
  delete prev_frame;
}

bool FramesNewPairwiseDistanceTransformer::transform(libfreenect2::FrameMap &input_frames,
                                                     libfreenect2::FrameMap &output_frames) {
  unsigned char *new_frame_data = input_frames[libfreenect2::Frame::Color]->data;

  if (has_prev) {
    unsigned char *prev_frame_data = prev_frame->data;
    libfreenect2::Frame *transformed_frame = new libfreenect2::Frame(prev_frame->width, prev_frame->height, 1);
    unsigned char transformed_frame_data[prev_frame->width * prev_frame->height];

    long sum, diff;
    int j;
    for (size_t i = 0; i < prev_frame->width * prev_frame->height; i++) {
      sum = 0;
      for (j = 0; j < prev_frame->bytes_per_pixel; j++) {
        diff = new_frame_data[i * prev_frame->bytes_per_pixel + j] -
               prev_frame_data[i * prev_frame->bytes_per_pixel + j];
        sum += diff * diff;
      }
      transformed_frame_data[i] = sqrt(sum);
    }
    transformed_frame->data = transformed_frame_data;
    output_frames[libfreenect2::Frame::Color] = transformed_frame;
  }

  memcpy(prev_frame->data, new_frame_data, prev_frame->width * prev_frame->height * prev_frame -> bytes_per_pixel);
  has_prev = true;
  return true;
}


FramesNewDownsizeTransformer::FramesNewDownsizeTransformer(int scale, size_t width, size_t height,
                                                           size_t bytes_per_pixel)
    : scale(scale),
      transformed_frame(new libfreenect2::Frame(width/scale, height/scale, bytes_per_pixel)) {
  unsigned char *transformed_frame_data;
  transformed_frame_data = new unsigned char[width / scale * height / scale * bytes_per_pixel];
  transformed_frame->data = transformed_frame_data;
}

FramesNewDownsizeTransformer::~FramesNewDownsizeTransformer() {
  delete transformed_frame->data;
  delete transformed_frame;
}

bool FramesNewDownsizeTransformer::transform(libfreenect2::FrameMap &input_frames,
                                             libfreenect2::FrameMap &output_frames) {
  libfreenect2::Frame *frame = input_frames[libfreenect2::Frame::Color];
  unsigned char *frame_data = frame->data;
  unsigned char *transformed_frame_data = transformed_frame->data;

  int i, j, k;
  for (i = 0; i < transformed_frame->height; i++) {
    for (j = 0; j < transformed_frame->width; j++) {
      for (k = 0; transformed_frame->bytes_per_pixel; k++) {
        transformed_frame_data[(i * transformed_frame->width + j) * transformed_frame->bytes_per_pixel + k] =
            frame_data[(i * frame->width + j) * transformed_frame->bytes_per_pixel * scale + k];
      }
    }
  }
  output_frames[libfreenect2::Frame::Color] = transformed_frame;
  return true;
}