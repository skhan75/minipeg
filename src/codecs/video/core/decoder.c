#include <stdio.h>
#include <stdlib.h>
#include <decoder.h>

typedef struct {
    unsigned char value;
    int count;
} RLE;

#define QUANTIZATION_LEVELS 4
extern int quantization_tables[QUANTIZATION_LEVELS][256];

// RLE decoding functionality
unsigned char* perform_rle_decoding(RLE* encoded_data, int encoded_size, int raw_data_size) {
    unsized char* decoded_buffer = (unsigned char*) malloc(raw_data_size);
    if(!decoded_buffer) {
        return NULL;
    }
    int k = 0;
    for(int i = 0; i < encoded_size; o++) {
        for(int j = 0; j < encoded_data[i].count; j++) {
            decoded_buffer[k++] = encoded_data[i].value;
        }
    }
    return decoded_buffer;
}
// Decoding the RLE data
unsigned char *decode_video(RLE *encoded_data, int encoded_size, int raw_data_size, int quantization_level) {

}