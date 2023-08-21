#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

typedef struct {
    int block_size;
    double **dct_matrix;
} DCTContext;

// P.S This is a foundational DCT implementation. The real magic and complexity come with optimizations 
// like fast cosine transforms, fixed-point arithmetic, SIMD instructions, and other hardware/software 
// optimizations.

// Initialize the DCT matrix
static void generate_dct_matrix(DCTContext *ctx) {
    double scaleFactor = sqrt(2.0 / BLOCK_SIZE);
    for(int i=0; i<ctx->block_size; i++) {
        for(int j=0; j<ctx->block_size; j++) {
            if(i==0) ctx->dct_matrix[i][j] = 1.0 / sqrt(ctx->block_size);
            else ctx->dct_matrix[i][j] = scaleFactor * cos((PI/(2.0*ctx->block_size)) * j*(2.0*i+1));
        }
    }
}

DCTContext *init_dct(int block_size) {
    DCTContext *ctx = (DCTContext *)malloc(sizeof(DCTContext));
    if(!ctx) return NULL;

    ctx->block_size = block_size;
    ctx->dct_matrix = (double **)malloc(sizeof(double *) * block_size);
    for(int i=0; i<block_size; i++) {
        ctx->dct_matrix[i] = (double *)malloc(sizeof(double) * block_size);
    }

    generate_dct_matrix(ctx);
    return ctx;
}

void free_dct(DCTContext *ctx) {
    for (int i = 0; i < ctx->block_size; i++) {
        free(ctx->dct_matrix[i]);
    }
    free(ctx->dct_matrix);
    free(ctx);
}

void perform_dct(DCTContext *ctx, double block[][ctx->block_size], double output[][ctx->block_size]) {
    double temp[ctx->block_size][ctx->block_size];

    // Multiply the block with the DCT matrix = DCT_MATRIX * block
    for (int i = 0; i < ctx->block_size; i++) {
        for (int j = 0; j < ctx->block_size; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < ctx->block_size; k++) {
                temp[i][j] += ctx->dct_matrix[i][k] * block[k][j];
            }
        }
    }

    // Multiply the result with the transpose of the DCT matrix = temp * DCT_MATRIX^T
    for (int i = 0; i < ctx->block_size; i++) {
        for (int j = 0; j < ctx->block_size; j++) {
            output[i][j] = 0;
            for (int k = 0; k < ctx->block_size; k++) {
                output[i][j] += temp[i][k] * ctx->dct_matrix[k][j];
            }
        }
    }
}

void quantize_dct_coefficients(DCTContext *ctx, double coefficients[][ctx->block_size], int quantization_table[][ctx->block_size]) {
    for (int i = 0; i < ctx->block_size; i++) {
        for (int j = 0; j < ctx->block_size; j++) {
            coefficients[i][j] /= quantization_table[i][j];
            coefficients[i][j] = round(coefficients[i][j]);
        }
    }
}