#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

typedef struct {
    unsigned char Y, Cb, Cr;
} YCbCrPixel;

void rgb_to_ycbcr(unsigned char r, unsigned char g, unsigned char b, YCbCrPixel* output_pixel);
void ycbcr_to_rgb(YCbCrPixel pixel, unsigned char* r, unsigned char* g, unsigned char* b);

#endif
