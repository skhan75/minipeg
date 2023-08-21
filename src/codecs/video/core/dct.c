#include <math.h>
#include <stdlib.h>

#define BLOCK_SIZE 8
#define PI 3.14159265358979323846

double DCT_MATRIX[BLOCK_SIZE][BLOCK_SIZE];

// P.S This is a foundational DCT implementation. The real magic and complexity come with optimizations 
// like fast cosine transforms, fixed-point arithmetic, SIMD instructions, and other hardware/software 
// optimizations.

// Initialize the DCT matrix
static void initialize_dct_matrix() {
    double scaleFactor = sqrt(2.0 / BLOCK_SIZE);
    for(int i=0; i<BLOCK_SIZE; i++) {
        for(int j=0; j<BLOCK_SIZE; j++) {
            if(i==0) DCT_MATRIX[i][j] = 1.0 / sqrt(BLOCK_SIZE);
            else DCT_MATRIX[i][j] = scaleFactor * cos((PI/(2.0*BLOCK_SIZE)) * j*(2.0*i+1));
        }
    }
}

void perform_dct(double block[BLOCK_SIZE][BLOCK_SIZE], double output[BLOCK_SIZE][BLOCK_SIZE]) {
    double temp[BLOCK_SIZE][BLOCK_SIZE] = {0};

    // Multiply the block with the DCT matrix = DCT_MATRIX * block
    for(int i=0; i<BLOCK_SIZE; i++) {
        for(int j=0; j<BLOCK_SIZE; j++) {
            for(int k=0; k<BLOCK_SIZE; k++) {
                temp[i][j] += DCT_MATRIX[i][k] * block[k][j];
            }
        }
    }

    // Multiply the result with the transpose of the DCT matrix = temp * DCT_MATRIX^T
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            for (int k = 0; k < BLOCK_SIZE; k++) {
                output[i][j] += temp[i][k] * DCT_MATRIX[k][j];
            }
        }
    }
}

void quantize_dct_coefficients(double coefficients[BLOCK_SIZE][BLOCK_SIZE], int quantization_table[BLOCK_SIZE][BLOCK_SIZE]) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            coefficients[i][j] /= quantization_table[i][j];
            coefficients[i][j] = round(coefficients[i][j]);
        }
    }
}