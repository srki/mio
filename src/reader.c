/* LICENSE PLACEHOLDER */

#include <mio/mio.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <mmio.h>

static void shuffleArray(int *array, size_t size) {
    srand(0);

    for (size_t i = 1; i < size; i++) {
        size_t swapIdx = rand() % (i + 1);

        int t = array[swapIdx];
        array[swapIdx] = array[i];
        array[i] = t;
    }
}

#define DEFINE_READ_TUPLES(NAME, TYPE, READ_TYPE, READ_TYPE_FORMAT)                     \
int mio_read_tuples_##NAME(const char *path, int *nrows_out, int *ncols_out,            \
                           int *nz_out, int **I_out, int **J_out, TYPE **X_out,         \
                           bool shuffle) {                                              \
    int nrows, ncols, nz;                                                               \
    int *I, *J;                                                                         \
    TYPE *X;                                                                            \
    READ_TYPE t;                                                                        \
                                                                                        \
    MM_typecode mat_code;                                                               \
    bool symmetric;                                                                     \
    char line[MM_MAX_LINE_LENGTH];                                                      \
    FILE *f;                                                                            \
    long fileOffset;                                                                    \
    int explicitValue;                                                                  \
                                                                                        \
    f = fopen(path, "r");                                                               \
    if (f == NULL) {                                                                    \
        fprintf(stderr, "Could not open file %s.\n", path);                             \
        return 1;                                                                       \
    }                                                                                   \
                                                                                        \
    if (mm_read_banner(f, &mat_code) != 0) {                                            \
        fprintf(stderr, "Could not process Matrix Market banner.\n");                   \
        return 2;                                                                       \
    }                                                                                   \
                                                                                        \
    if (mm_is_complex(mat_code) && mm_is_matrix(mat_code) && mm_is_sparse(mat_code)) {  \
        fprintf(stderr, "Sorry, this application does not support ");                   \
        fprintf(stderr, "Market Market type: [%s]\n", mm_typecode_to_str(mat_code));    \
        return 3;                                                                       \
    }                                                                                   \
                                                                                        \
    if (mm_read_mtx_crd_size(f, &nrows, &ncols, &nz) != 0) {                            \
        return 4;                                                                       \
    }                                                                                   \
                                                                                        \
    symmetric = mm_is_symmetric(mat_code);                                              \
                                                                                        \
    I = malloc(nz * sizeof(int) * (symmetric ? 2 : 1));                                 \
    J = malloc(nz * sizeof(int) * (symmetric ? 2 : 1));                                 \
    X = malloc(nz * sizeof(TYPE) * (symmetric ? 2 : 1));                                \
    if (I == NULL || J == NULL || X == NULL) {                                          \
        free(I);                                                                        \
        free(J);                                                                        \
        free(X);                                                                        \
        return 5;                                                                       \
    }                                                                                   \
                                                                                        \
    /* Determine if value is implicit or explicit */                                    \
    fileOffset = ftell(f);                                                              \
    char *pc_unused = fgets(line, MM_MAX_LINE_LENGTH, f);                               \
    if (sscanf(line, "%d %d %" READ_TYPE_FORMAT "\n", I, J, &t) == 3) {                 \
        explicitValue = 1;                                                              \
    } else {                                                                            \
        explicitValue = 0;                                                              \
    }                                                                                   \
    fseek(f, fileOffset, SEEK_SET);                                                     \
                                                                                        \
    /* Read values from the file */                                                     \
    for (int i = 0; i < nz; i++) {                                                      \
        int i_unused = fscanf(f, "%d %d", &I[i], &J[i]);                                \
        I[i]--;                                                                         \
        J[i]--;                                                                         \
        if (symmetric) {                                                                \
            I[i + nz] = J[i];                                                           \
            J[i + nz] = I[i];                                                           \
        }                                                                               \
                                                                                        \
        if (explicitValue) {                                                            \
            i_unused = fscanf(f, "%" READ_TYPE_FORMAT, &t);                             \
            X[i] = t;                                                                   \
            if (symmetric) { X[i + nz] = t; }                                           \
        } else {                                                                        \
            X[i] = 1;                                                                   \
            if (symmetric) { X[i + nz] = 1; }                                           \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    if (symmetric) { nz *= 2; }                                                         \
                                                                                        \
    if (shuffle) {                                                                      \
        int *permutation = malloc(sizeof(int) * nrows);                                 \
        for (int i = 0; i < nrows; i++) { permutation[i] = i; }                         \
        shuffleArray(permutation, nrows);                                               \
        for (int i = 0;i < nz; i++) {                                                   \
            I[i] = permutation[I[i]];                                                   \
            J[i] = permutation[J[i]];                                                   \
            X[i] = X[i];                                                                \
        }                                                                               \
        free(permutation);                                                              \
    }                                                                                   \
                                                                                        \
    /* Write out values */                                                              \
    *nrows_out = nrows;                                                                 \
    *ncols_out = ncols;                                                                 \
    *nz_out = nz;                                                                       \
    *I_out = I;                                                                         \
    *J_out = J;                                                                         \
    *X_out = X;                                                                         \
                                                                                        \
    return 0;                                                                           \
}


/* @formatter:off */

DEFINE_READ_TUPLES(BOOL,   bool,     uint32_t, PRIu32)
DEFINE_READ_TUPLES(UINT8,  uint8_t,  uint32_t, PRIu32)
DEFINE_READ_TUPLES(INT8,   int8_t,   int32_t,  PRId32)
DEFINE_READ_TUPLES(UINT16, uint16_t, uint32_t, PRIu32)
DEFINE_READ_TUPLES(INT16,  int16_t,  int32_t,  PRId32)
DEFINE_READ_TUPLES(UINT32, uint32_t, uint32_t, PRIu32)
DEFINE_READ_TUPLES(INT32,  int32_t,  int32_t,  PRId32)
DEFINE_READ_TUPLES(UINT64, uint64_t, uint64_t, PRIu64)
DEFINE_READ_TUPLES(INT64,  int64_t,  int64_t,  PRId64)
DEFINE_READ_TUPLES(FP32,   float,    float , "f")
DEFINE_READ_TUPLES(FP64,   double,   double, "lf")

/* @formatter:on */
