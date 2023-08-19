#include <stdio.h>
#include <stdlib.h>
#include "encoder.h"
#include "decoder.h"
#include "video_compression_tool.h"

void compress_video(const char* input_filename, const char* output_filename, RawVideoConfig* config, int quantization_level) {
    FILE *infile, *outfile;
    unsigned char* raw_buffer;
    long file_size;

    // Open the raw video file for reading
    infile = fopen(input_filename, "rb");
    if(!infile) {
        perror("Failed to open file.\n");
        return;
    }

    // Determine the file size
    fseek(infile, 0L, SEEK_END);
    file_size = ftell(infile);
    rewind(infile);

    // Allocate memory and read the raw video
    // TODO - This is reading the entire raw video file into the memory.
    // which is not efficient for large video files. We should consider
    // streaming the data instead. (TBD in a future iteration)
    raw_buffer = (unsigned char*) malloc(file_size);
    if(!raw_buffer) {
        perror("Failed to allocate memory.\n");
        fclose(infile);
        return;
    }
    fread(raw_buffer, 1, file_size, infile);
    fclose(infile);

    // Encode the video
    EncodedVideoData* encoded_data = encode_video(raw_buffer, file_size, config, quantization_level);
    if(!encoded_data) {
        perror("Failed to encode video.\n");
        free(raw_buffer);
        return;
    }

    // Write the encoded data to the output file
    outfile = fopen(output_filename, "wb");
    if(!outfile) {
        perror("Failed to open output file.\n");
        free(raw_buffer);
        free(encoded_data);
        return;
    }

    fwrite(encoded_data->data, sizeof(RLE), encoded_data->data_size, outfile);
    fclose(outfile);

    free(raw_buffer);
    free(encoded_data->data);
    free(encoded_data);
}

void decompress_video(const char* input_filename, const char* output_filename, RawVideoConfig* config) {
    FILE* infile = fopen(input_filename, "rb");
    if (!infile) {
        perror("Failed to open input file for decompression");
        return;
    }

    FILE* outfile = fopen(output_filename, "wb");
    if (!outfile) {
        perror("Failed to open output file for decompression");
        fclose(infile);
        return;
    }

    // Read compressed video data from the input file
    fseek(infile, 0, SEEK_END);
    long file_size = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    EncodedVideoData encoded_data;
    encoded_data.data = (RLE*)malloc(file_size);
    if (!encoded_data.data) {
        perror("Memory allocation failed for decompression");
        fclose(infile);
        fclose(outfile);
        return;
    }

    fread(encoded_data.data, 1, file_size, infile);

    // This assumes the encoded data type and quantization level were serialized at the end of the compressed file.
    // If you've serialized them differently, adjust accordingly.
    encoded_data.encoding_type = encoded_data.data[file_size - 2].value;
    encoded_data.quantization_level = encoded_data.data[file_size - 1].value;
    encoded_data.data_size = file_size - 2;  // excluding the 2 bytes for encoding_type and quantization_level

    // Decode the video
    unsigned char* decompressed_buffer = decode_video(&encoded_data, config);
    
    if (!decompressed_buffer) {
        fprintf(stderr, "Failed to decode video.\n");
        free(encoded_data.data);
        fclose(infile);
        fclose(outfile);
        return;
    }

    // Write the decoded video data to the output file
    int raw_video_size = config->width * config->height * config->bytes_per_pixel * config->frame_count;
    fwrite(decompressed_buffer, 1, raw_video_size, outfile);

    // Clean up
    free(encoded_data.data);
    free(decompressed_buffer);
    fclose(infile);
    fclose(outfile);
}