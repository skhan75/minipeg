#include <stdio.h>
#include <stdlib.h>
#include <encoder.h>
#include "video_compression_tool.h"

#define MOCK_VIDEO_FILE "mock_video.raw"
#define COMPRESSED_VIDEO_FILE "compressed_video.dat"

void generate_mock_video(const char* filename, int width, int height, int bytes_per_pixel, int frame_count) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to create mock video file");
        exit(1);
    }

    // Generate mock video data with repetitive patterns.
    unsigned char pattern[bytes_per_pixel];
    for (int i = 0; i < bytes_per_pixel; i++) {
        pattern[i] = (unsigned char)(i * 50); // Some arbitrary value.
    }

    int frame_size = width * height * bytes_per_pixel;
    for (int f = 0; f < frame_count; f++) {
        for (int i = 0; i < frame_size; i += bytes_per_pixel) {
            fwrite(pattern, 1, bytes_per_pixel, file);
        }
    }

    fclose(file);
}

int main() {
    RawVideoConfig config;
    config.width = 320;
    config.height = 240;
    config.bytes_per_pixel = 3; // Assume RGB format for our mock video.
    config.frame_count = 10; // 10 frames for our mock video.

    generate_mock_video(MOCK_VIDEO_FILE, config.width, config.height, config.bytes_per_pixel, config.frame_count);
    
    compress_video(MOCK_VIDEO_FILE, COMPRESSED_VIDEO_FILE, &config, 2);

    // Basic validation: Check that compressed file exists and is not empty.
    FILE* file = fopen(COMPRESSED_VIDEO_FILE, "rb");
    if (!file) {
        printf("Test failed: Couldn't open compressed video file.\n");
        return 1;
    }
    fseek(file, 0L, SEEK_END);
    long compressed_size = ftell(file);
    fclose(file);

    if (compressed_size > 0) {
        printf("Test passed: Compressed video file is not empty.\n");
        return 0;
    } else {
        printf("Test failed: Compressed video file is empty.\n");
        return 1;
    }
}
