#ifndef VIDEO_COMPRESSION_TOOL_H
#define VIDEO_COMPRESSION_TOOL_H

#include "encoder.h"

// Compresses the raw video file using the encoder and writes the compressed data to the output file.
void compress_video(const char *input_filename, const char *output_filename, RawVideoConfig *config, int quantization_level);

#endif // VIDEO_COMPRESSION_TOOL_H
