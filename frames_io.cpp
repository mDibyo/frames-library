#include "frames_io.h"

#include <fstream>

#ifdef SAVE_WITH_LIBPNG
#include <png.h>
#endif


FramesInputterFromDevice::FramesInputterFromDevice()
    : listener(new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color)) {
  if (freenect2.enumerateDevices() == 0) {
    std::cout << "no devices connected!" << std::endl;
    throw FramesIOException("no device connected!");
  }

  std::string serial = freenect2.getDefaultDeviceSerialNumber();
  device = freenect2.openDevice(serial);
  if (device == 0) {
    std::cout << "failure opening device!" << std::endl;
    throw FramesIOException("failure opening device!");
  }

  device->setColorFrameListener(listener);
  device->start();
}

FramesInputterFromDevice::~FramesInputterFromDevice() {
  if (device == 0) {
    return;
  }
  device->stop();
  device->close();
}

bool FramesInputterFromDevice::getNextFrame(libfreenect2::FrameMap &frames) {
  listener->waitForNewFrame(frames);
  return true;
}


FramesInputterFromDisk::FramesInputterFromDisk(
    std::string prefix, size_t width, size_t height, size_t bytes_per_pixel)
    : input_prefix(prefix),
      current_frame_idx(-1),
      frame(new libfreenect2::Frame(width, height, bytes_per_pixel)) { }

FramesInputterFromDisk::~FramesInputterFromDisk() {
  delete frame;
}

bool FramesInputterFromDisk::getNextFrame(libfreenect2::FrameMap &frames) {
  current_frame_idx += 1;

  char filename[256];
  std::sprintf(filename, (input_prefix + "%d").c_str(), current_frame_idx);
  std::cout << filename << std::endl;

  std::ifstream file(filename, std::ios::binary|std::ios::ate);
  if (!file.is_open()) {
    std::cout << "file could not be opened for reading" << std::endl;
    return false;
  }

  int exp_filesize = frame->width * frame->height * frame->bytes_per_pixel;
  if (file.tellg() != exp_filesize) {
    std::cout << "file not of the correct size" << std::endl;
    return false;
  }

  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char *>(frame->data), exp_filesize);
  frames[libfreenect2::Frame::Color] = frame;

  file.close();
  return true;
}


FramesOutputterToDisk::FramesOutputterToDisk(std::string prefix)
    : output_prefix(prefix),
      current_frame_idx(-1) { }

bool FramesOutputterToDisk::putNextFrame(libfreenect2::FrameMap & frames) {
  if (frames.count(libfreenect2::Frame::Color) == 0) {
    std::cout << "color frame not present" << std::endl;
    return false;
  }

  current_frame_idx += 1;

  char filename[256];
  std::sprintf(filename, (output_prefix + "%d").c_str(), current_frame_idx);
  std::cout << filename << std::endl;

  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cout << "file could not be opened for writing" << std::endl;
    return false;
  }

  libfreenect2::Frame *frame = frames[libfreenect2::Frame::Color];
  int filesize = frame->width * frame->height * frame->bytes_per_pixel;
  file.write(reinterpret_cast<const char *>(frame->data), filesize);

  file.close();
  return true;
}


#ifdef SAVE_WITH_LIBPNG
FramesOutputterToDiskPNG::FramesOutputterToDiskPNG(std::string prefix)
    : output_prefix(prefix),
      current_frame_idx(-1) { }

bool FramesOutputterToDiskPNG::putNextFrame(libfreenect2::FrameMap &frames) {
  if (frames.count(libfreenect2::Frame::Color) == 0) {
    std::cout << "color frame not present" << std::endl;
    return false;
  }

  current_frame_idx += 1;

  char filename[256];
  std::sprintf(filename, (output_prefix + "%d.png").c_str(), current_frame_idx);
  std::cout << filename << std::endl;

  FILE *file = fopen(filename, "wb");
  if (!file) {
    std::cout << "file could not be opened for writing" << std::endl;
    return false;
  }

  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fclose(file);
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(file);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(file);
    return false;
  }

  png_init_io(png_ptr, file);

  libfreenect2::Frame *frame = frames[libfreenect2::Frame::Color];
  png_set_IHDR(png_ptr, info_ptr, frame->width, frame->height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  png_set_bgr(png_ptr);

  png_bytep row_pointers[frame->height];
  for (int k = 0; k < frame->height; k++) {
    row_pointers[k] = frame->data + k * frame->width * frame->bytes_per_pixel;
  }
  png_write_image(png_ptr, row_pointers);

  png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
  fclose(file);
  return true;
}

#endif // SAVE_WITH_LIBPNG