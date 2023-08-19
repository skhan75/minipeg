#include <stdio.h>
#include <stdlib.h>
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

// TODO - In the future, also add a decompress_video function here to reverse the process.