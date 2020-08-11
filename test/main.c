/* LICENSE PLACEHOLDER */

#include <mio/mio.h>
#include <stdio.h>

int main() {

    int nrows, ncols, nnz;
    int *I, *J;
    bool *X;

    mio_read_tuples_BOOL(INPUTS_DIR"/logo.mtx", &nrows, &ncols, &nnz, &I, &J, &X, false);

    for (int i = 0; i < nnz; i++) {
        printf("%d %d\n", I[i], J[i]);
    }

    mio_free_tuples(I, J, X);
    return 0;
}

