#include <math.h>
#include <stdlib.h>
#include "dct.h"

// P.S This is a foundational DCT implementation. The real magic and complexity come with optimizations 
// like fast cosine transforms, fixed-point arithmetic, SIMD instructions, and other hardware/software 
// optimizations.

// Initialize the DCT matrix
DCTContext* init_dct(int block_size) {
    DCTContext* ctx = malloc(sizeof(DCTContext));
    if (!ctx) return NULL;

    ctx->block_size = block_size;

    ctx->dct_matrix = malloc(block_size * sizeof(double*));
    ctx->dct_matrix_transpose = malloc(block_size * sizeof(double*));

    // if memory allocation fails
    if (!ctx->dct_matrix || !ctx->dct_matrix_transpose) {
        free(ctx);
        return NULL;
    }

    for (int i = 0; i < block_size; i++) {
        ctx->dct_matrix[i] = malloc(block_size * sizeof(double));
        ctx->dct_matrix_transpose[i] = malloc(block_size * sizeof(double));
        if (!ctx->dct_matrix[i] || !ctx->dct_matrix_transpose[i]) {
            // Handle the memory allocation failure for these blocks here.
            // One simple way is to free everything that has been allocated so far and then return NULL.
            for (int j = 0; j <= i; j++) {
                free(ctx->dct_matrix[j]);
                free(ctx->dct_matrix_transpose[j]);
            }
            free(ctx->dct_matrix);
            free(ctx->dct_matrix_transpose);
            free(ctx);
            return NULL;
        }
    }

    double scaleFactor = sqrt(2.0 / block_size);
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (i == 0)
                ctx->dct_matrix[i][j] = 1.0 / sqrt(block_size);
            else
                ctx->dct_matrix[i][j] = scaleFactor * cos((PI / (2.0 * block_size)) * j * (2.0 * i + 1));
            ctx->dct_matrix_transpose[j][i] = ctx->dct_matrix[i][j]; // Transpose
        }
    }
    return ctx;
}

void perform_dct(DCTContext* ctx, unsigned char** block, double** output) {
    int block_size = ctx->block_size;
    double temp[block_size][block_size];

    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            temp[i][j] = 0.0;
            for (int k = 0; k < block_size; k++) {
                temp[i][j] += ctx->dct_matrix[i][k] * block[k][j];
            }
        }
    }

    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            output[i][j] = 0.0;
            for (int k = 0; k < block_size; k++) {
                output[i][j] += temp[i][k] * ctx->dct_matrix_transpose[k][j];
            }
        }
    }
}

void quantize_dct_coefficients(double** coefficients, int** quantization_table, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            coefficients[i][j] /= quantization_table[i][j];
            coefficients[i][j] = round(coefficients[i][j]);
        }
    }
}

void free_dct(DCTContext* ctx) {
    int block_size = ctx->block_size;
    for (int i = 0; i < block_size; i++) {
        free(ctx->dct_matrix[i]);
        free(ctx->dct_matrix_transpose[i]);
    }
    free(ctx->dct_matrix);
    free(ctx->dct_matrix_transpose);
    free(ctx);
}