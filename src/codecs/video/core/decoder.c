#include <stdio.h>
#include <stdlib.h>
#include <decoder.h>

typedef struct {
    unsigned char value;
    int count;
} RLE;

// Dequantization function to restore pixel values from their quantized forms.
// It undoes the effects of the quantization step done during encoding.
void apply_dequantization(unsigned char *buffer, int buffer_size, int quantization_level) {
    // Loop through each byte in the buffer
    for (int i = 0; i < buffer_size; i++) {
        // Restore the original value range by multiplying with the quantization level.
        buffer[i] = (unsigned char)((buffer[i] + (quantization_level / 2)) / quantization_level * quantization_level);
    }
}

// RLE decoding functionality
unsigned char *perform_rle_decoding(RLE *encoded_data, int encoded_size, int *decoded_size) {
    // Calculate the total decoded size by summing up the counts of all RLE entries.
    *decoded_size = 0;
    for (int i = 0; i < encoded_size; i++) {
        *decoded_size += encoded_data[i].count;
    }

    // Allocate memory for the decoded data
    unsigned char *decoded_buffer = (unsigned char *)malloc(*decoded_size);
    if (!decoded_buffer) return NULL;

    int j = 0; // Index for decoded_buffer
    // Loop through the encoded data
    for (int i = 0; i < encoded_size; i++) {
        // For each RLE entry, copy the value 'count' times to the decoded buffer.
        for (int k = 0; k < encoded_data[i].count; k++) {
            decoded_buffer[j++] = encoded_data[i].value;
        }
    }
    return decoded_buffer;
}

// Decoding the RLE data
unsigned char *decode_video(EncodedVideoData *encoded_data, RawVideoConfig *config) {
    if(encoded_data->encoding_type != RLE_ENCODING) {
        // unsupported encoding type
        return NULL;
    }

    int decoded_size;
    unsigned char* decoded_buffer = perform_rle_decoding(encoded_data->data, encoded_data->data_size, &decoded_size);
    if(!decoded_buffer) return NULL;
    

    apply_dequantization(decoded_buffer, decoded_size, encoded_data->quantization_level);
    return decoded_buffer
}