// This file contains the implementation of the raw video decoder.
// Raw video refers to video data that hasn't neem compressed or processed
// using any codec. It's simply a representation of the raw pixel data, frame
// by frame.
// The purpose of this decoder is to provide functionality to decode (or read)
// raw video data from a file and return the raw pixel data in a buffer.

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
 * */
unsigned char *load_raw_video(const char *filename, RawVideoConfig *config) {
  FILE* file = fopen(filename, "rb");
  if (!file) return NULL;

  // Frame size = width * height * color depth
  int frame_size = config->width * config->height * config->bytes_per_pixel;
  // Allocate memory for all the frames
  int total_size = frame_size * config->frame_count;

  // Allocate memory buffer to hold the frame data
  unsigned char* buffer = (unsigned char*) malloc(frame_size);
  if(!buffer) {
    fclose(file);
    return NULL;
  }

  // TODO - currently reading the entire content of the video file in one go. 
  // This is efficient but assumes that entire video file can fit in memory.
  // For large video files, we should consider alternative data structures or
  // streaming mechanisms.
  size_t bytesRead = fread(buffer, 1, total_size, file);
  if(bytesRead != total_size) {
    // Error handling - could not read all the frames
    free(buffer);
    fclose(file);
    return NULL;
  }

  fclose(file);

  return buffer;
}
