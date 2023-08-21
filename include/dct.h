#ifndef DCT_H
#define DCT_H

#define PI 3.14159265358979323846

// Define a structure for DCT context
typedef struct {
    // ... (any necessary context data)
    int block_size;
    double** dct_matrix;                  // Using pointers for dynamic allocation
    double** dct_matrix_transpose;
} DCTContext;

// Function to initialize DCT
DCTContext* init_dct(int block_size);

// The core DCT function
void perform_dct(DCTContext* ctx, unsigned char** block, double** output);

// Function to quantize DCT coefficients
void quantize_dct_coefficients(double **coefficients, int **quantization_table, int block_size);

// Clean up function (e.g., to free any memory you allocated in init_dct)
void free_dct(DCTContext* ctx);

#endif // DCT_H
