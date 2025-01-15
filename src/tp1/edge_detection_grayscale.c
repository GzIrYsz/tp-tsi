#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_img = LoadPGM_bmatrix("data/input/cubes.pgm", &nrl, &nrh, &ncl, &nch);
    byte **output_img_horizontal_gradient = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_vertical_gradient = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_horizontal_vertical_gradient = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection = bmatrix(nrl, nrh, ncl, nch);

    int horizontal_sum;
    int vertical_sum;
    int gradient_norm;
    for (int i = nrl + 1; i < nrh; i++) {
        for (int j = ncl + 1; j < nch; j++) {
            horizontal_sum = input_img[i - 1][j - 1] * -1
                            + input_img[i][j - 1] * 0
                            + input_img[i + 1][j - 1] * 1
                            + input_img[i - 1][j] * -2
                            + input_img[i][j] * 0
                            + input_img[i + 1][j] * 2
                            + input_img[i - 1][j + 1] * -1
                            + input_img[i][j + 1] * 0
                            + input_img[i + 1][j + 1] * 1;
            horizontal_sum /= 4;

            vertical_sum = input_img[i - 1][j - 1] * -1
                            + input_img[i][j - 1] * -2
                            + input_img[i + 1][j - 1] * -1
                            + input_img[i - 1][j] * 0
                            + input_img[i][j] * 0
                            + input_img[i + 1][j] * 0
                            + input_img[i - 1][j + 1] * 1
                            + input_img[i][j + 1] * 2
                            + input_img[i + 1][j + 1] * 1;
            vertical_sum /= 4;

            output_img_horizontal_gradient[i][j] = abs(horizontal_sum);
            output_img_vertical_gradient[i][j] = abs(vertical_sum);

            gradient_norm = (int) sqrt(pow(horizontal_sum, 2) + pow(vertical_sum, 2));
            output_img_horizontal_vertical_gradient[i][j] = gradient_norm;
            output_img_edges_detection[i][j] = gradient_norm > 15 ? 255 : 0;
        }
    }
    SavePGM_bmatrix(output_img_horizontal_gradient, nrl, nrh, ncl, nch, "data/output/cubes_horizontal_gradient.pgm");
    SavePGM_bmatrix(output_img_vertical_gradient, nrl, nrh, ncl, nch, "data/output/cubes_vertical_gradient.pgm");
    SavePGM_bmatrix(output_img_horizontal_vertical_gradient, nrl, nrh, ncl, nch, "data/output/cubes_horizontal_vertical_gradient.pgm");
    SavePGM_bmatrix(output_img_edges_detection, nrl, nrh, ncl, nch, "data/output/cubes_edges_detection.pgm");

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_horizontal_gradient, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_vertical_gradient, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_horizontal_vertical_gradient, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection, nrl, nrh, ncl, nch);
    return EXIT_SUCCESS;
}