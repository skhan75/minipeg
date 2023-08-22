#include "color_conversion.h"

void ycbcr_to_rgb(YCbCrPixel pixel, unsigned char* r, unsigned char* g, unsigned char* b) {
    double Y = (double)pixel.Y;
    double Cb = (double)pixel.Cb - 128.0;
    double Cr = (double)pixel.Cr - 128.0;

    double R = Y + 1.402 * Cr;
    double G = Y - 0.344136 * Cb - 0.714136 * Cr;
    double B = Y + 1.772 * Cb;

    // Clipping to ensure values remain in the 0-255 range
    *r = (unsigned char)(R < 0 ? 0 : R > 255 ? 255 : R);
    *g = (unsigned char)(G < 0 ? 0 : G > 255 ? 255 : G);
    *b = (unsigned char)(B < 0 ? 0 : B > 255 ? 255 : B);
}

void rgb_to_ycbcr(
    unsigned char R, 
    unsigned char G, 
    unsigned char B, 
    unsigned char *Y,
    unsigned char *Cb,
    unsigned char *Cr
) {
    *Y = (unsigned char)(0.299 * R + 0.587 * G + 0.114 * B);
    *Cb = (unsigned char)(128 - 0.168736 * R - 0.331264 * G + 0.5 * B);
    *Cr = (unsigned char)(128 + 0.5 * R - 0.418688 * G - 0.081312 * B);
}