#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

#define STR_(X) #X
#define STR(X) STR_(X)

#define WHITE_THRESHOLD 110

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_img = LoadPGM_bmatrix("data/input/rice.pgm", &nrl, &nrh, &ncl, &nch);
    byte **output_img = bmatrix(nrl, nrh, ncl, nch);

    int i, j;

    int sum;
    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            sum = (input_img[i - 1][j - 1] * 1
                + input_img[i - 1][j] * 1
                + input_img[i - 1][j + 1] * 1
                + input_img[i][j - 1] * 1
                + input_img[i][j] * 1
                + input_img[i][j + 1] * 1
                + input_img[i + 1][j - 1] * 1
                + input_img[i + 1][j] * 1
                + input_img[i + 1][j + 1] * 1) / 9;

            output_img[i][j] = sum > WHITE_THRESHOLD ? 255 : 0;
        }
    }

    SavePGM_bmatrix(output_img, nrl, nrh, ncl, nch, "data/output/rice_binarized_avg_t" STR(WHITE_THRESHOLD) ".pgm");

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(output_img, nrl, nrh, ncl, nch);
    return EXIT_SUCCESS;
}