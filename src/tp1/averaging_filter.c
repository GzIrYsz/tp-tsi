#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_img = LoadPGM_bmatrix("data/input/cubes.pgm", &nrl, &nrh, &ncl, &nch);
    byte **output_img = bmatrix(nrl, nrh, ncl, nch);

    for (int i = nrl + 1; i < nrh; i++) {
        for (int j = ncl + 1; j < nch; j++) {
            output_img[i][j] = (input_img[i - 1][j - 1] * 1
                                + input_img[i - 1][j] * 1
                                + input_img[i - 1][j + 1] * 1
                                + input_img[i][j - 1] * 1
                                + input_img[i][j] * 1
                                + input_img[i][j + 1] * 1
                                + input_img[i + 1][j - 1] * 1
                                + input_img[i + 1][j] * 1
                                + input_img[i + 1][j + 1] * 1) / 9;
        }
    }

    SavePGM_bmatrix(output_img, nrl, nrh, ncl, nch, "data/output/cubes_averaging_filter_3x3.pgm");

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(output_img, nrl, nrh, ncl, nch);
    return EXIT_SUCCESS;
}