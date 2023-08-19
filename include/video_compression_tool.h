// video_compression_tool.h

#ifndef VIDEO_COMPRESSION_TOOL_H
#define VIDEO_COMPRESSION_TOOL_H

// Compresses a raw video file and writes the compressed data to an output file.
void compress_video(const char* input_filename, const char* output_filename, RawVideoConfig* config, int quantization_level);

// Decompresses a compressed video file and writes the decompressed data to an output file.
void decompress_video(const char* input_filename, const char* output_filename, RawVideoConfig* config);

#endif // VIDEO_COMPRESSION_TOOL_H
