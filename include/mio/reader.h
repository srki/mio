/* LICENSE PLACEHOLDER */

#ifndef GRB_FUSION_READER_H
#define GRB_FUSION_READER_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


#define DECLARE_READ_TUPLES(NAME, TYPE, READ_TYPE, READ_TYPE_FORMAT)                        \
int mio_read_tuples_##NAME(const char *path, int *nrows_out, int *ncols_out, int *nz_out,   \
                       int **I_out, int **J_out, TYPE **X_out, bool shuffle);

/* @formatter:off */
DECLARE_READ_TUPLES(BOOL,   bool,     uint32_t, PRIu32)
DECLARE_READ_TUPLES(UINT8,  uint8_t,  uint32_t, PRIu32)
DECLARE_READ_TUPLES(INT8,   int8_t,   int32_t,  PRId32)
DECLARE_READ_TUPLES(UINT16, uint16_t, uint32_t, PRIu32)
DECLARE_READ_TUPLES(INT16,  int16_t,  int32_t,  PRId32)
DECLARE_READ_TUPLES(UINT32, uint32_t, uint32_t, PRIu32)
DECLARE_READ_TUPLES(INT32,  int32_t,  int32_t,  PRId32)
DECLARE_READ_TUPLES(UINT64, uint64_t, uint64_t, PRIu64)
DECLARE_READ_TUPLES(INT64,  int64_t,  int64_t,  PRId64)
DECLARE_READ_TUPLES(FP32,   float,    float , "f")
DECLARE_READ_TUPLES(FP64,   double,   double, "lf")
/* @formatter:on */

static void mio_free_tuples(void *I, void *J, void *X) {
    free(I);
    free(J);
    free(X);
}

#ifdef __cplusplus
}
#endif

#endif //GRB_FUSION_READER_H
