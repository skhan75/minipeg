#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int width;
  int height;
  int bytes_per_pixel; // e.g., 3 for RGB, 4 for RGBA
  int frame_count;
} RawVideoConfig;

/*
 *  Returns pointer to a buffer containing the video frame data.
 *  This function only reads one frame at a time
 * */
unsigned char* decode_raw_video(const char* filename, RawVideoConfig* config) {
  FILE* file = fopen(filename, "rb");
  if (!file) return NULL;

  // Frame size = width * height * color depth
  int frame_size = config->width * config->height * config->bytes_per_pixel;
  int total_size = frame_size * config->frame_count;

  // Allocate memory buffer to hold the frame data
  unsigned char* buffer = (unsigned char*) malloc(frame_size);
  if(!buffer) {
    fclose(file);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, total_size, file);
  if(bytesRead != total_size) {
    // Error handling - could not read all the frames
  }
  fread(buffer, 1, frame_size, file);
  fclose(file);

  return buffer;
}

