#ifndef DECODER_H
#define DECODER_H

#include "encoder.h" // To access EncodedVideoData and RawVideoConfig structures

#define RLE_ENCODING 1

// Function to decode an encoded video back to its raw form (or closer to the original)
// The decoding will depend on the encoding_type and any associated parameters, like quantization_level.
unsigned char *decode_video(EncodedVideoData *encoded_data, RawVideoConfig *config);

unsigned char *perform_rle_decoding(RLE *encoded_data, int encoded_size, int *decoded_size);

#endif // DECODER_H