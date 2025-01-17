#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_image = LoadPGM_bmatrix("data/output/route0_binarized_avg_t130.pgm", &nrl, &nrh, &ncl, &nch);

    int width = nch - ncl;
    int height = nrh - nrl;
    int diag = sqrt(pow(width, 2) + pow(height, 2));

    byte **hough_buildup_table = bmatrix(0, diag, 0, 180);

    zero_bmatrix(hough_buildup_table, 0, diag, 0, 180);

    int i, j;

    int angle, rho;
    double phi;
    for (i = nrl; i <= nrh; i++) {
        for (j = ncl; j <= nch; j++) {
            if (input_image[i][j]) {
                for (angle = 0; angle <= 180; angle++) {
                    phi = M_PI * angle / 180;
                    rho = j * sin(phi) + i * cos(phi);

                    if (rho >= 0) hough_buildup_table[rho][angle]++;
                }
            }
        }
    }

    SavePGM_bmatrix(hough_buildup_table, 0, diag, 0, 180, "data/output/route0_hough_accumulation_table.pgm");

    free_bmatrix(input_image, nrl, nrh, ncl, nch);
    free_bmatrix(hough_buildup_table, 0, diag, 0, 180);
    return EXIT_SUCCESS;
}