#include "encoder.h"
#include <stdio.h>

int main() {
    // 1. Open the raw video file
    FILE *file = fopen("vid.y4m", "rb");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    // 2. Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 3. Read the video data into a buffer
    unsigned char *buffer = (unsigned char *)malloc(file_size);
    fread(buffer, 1, file_size, file);
    fclose(file);

    // 4. Set up configuration (assuming you know the video's properties)
    RawVideoConfig config;
    config.width = 1920;        // Example width
    config.height = 1080;       // Example height
    config.bytes_per_pixel = 3; // Assuming RGB
    config.frame_count = 1;     // Assuming one frame for simplicity

    // 5. Test the encoder
    int encoded_size;
    RLE *encoded_data = encode_video(buffer, file_size, &config, &encoded_size, 2); // Using quantization level 2 as an example

    // Cleanup and output info
    free(buffer);
    printf("Original size: %ld bytes\n", file_size);
    printf("Encoded size: %d RLE entries\n", encoded_size);

    // Normally you'd write the encoded_data somewhere or further process it
    free(encoded_data);

    return 0;
}
