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

void argmax_2d(byte **table, int num_rows, int num_cols, int *rowi, int *coli) {
    *rowi = 0;
    *coli = 0;

    int i, j;
    for (i = 1; i < num_rows; i++) {
        for (j = 1; j < num_cols; j++) {
            if (table[i][j] > table[*rowi][*coli]) {
                *rowi = i;
                *coli = j;
            }
        }
    }
}

void two_argmax_2d(byte **table, int num_rows, int num_cols, int *rowi1, int *coli1, int *rowi2, int *coli2) {
    *rowi1 = 0;
    *coli1 = 0;
    *rowi2 = 0;
    *coli2 = 0;

    int i, j;
    for (i = 1; i < num_rows; i++) {
        for (j = 1; j < num_cols; j++) {
            if (table[i][j] > table[*rowi1][*coli1]) {
                *rowi1 = i;
                *coli1 = j;
            }
            if (table[i][j] < table[*rowi1][*coli1] && table[i][j] > table[*rowi2][*coli2]) {
                *rowi2 = i;
                *coli2 = j;
            }
        }
    }
}

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_img = LoadPGM_bmatrix("data/input/route0.pgm", &nrl, &nrh, &ncl, &nch);
    byte **binarized = bmatrix(nrl, nrh, ncl, nch);
    rgb8 **output_image = rgb8matrix(nrl, nrh, ncl, nch);

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

            binarized[i][j] = sum > WHITE_THRESHOLD ? 255 : 0;
        }
    }

    int width = nch - ncl;
    int height = nrh - nrl;
    int diag = sqrt(pow(width, 2) + pow(height, 2));

    byte **hough_buildup_table = bmatrix(0, diag, 0, 180);

    zero_bmatrix(hough_buildup_table, 0, diag, 0, 180);

    int angle, rho;
    double phi;
    for (i = nrl; i <= nrh; i++) {
        for (j = ncl; j <= nch; j++) {
            if (binarized[i][j]) {
                for (angle = 0; angle <= 180; angle++) {
                    phi = M_PI * angle / 180;
                    rho = j * sin(phi) + i * cos(phi);

                    if (rho >= 0) hough_buildup_table[rho][angle]++;
                }
            }
        }
    }

    int rho1_max, phi1_max, rho2_max, phi2_max;
    two_argmax_2d(hough_buildup_table, diag + 1, 180, &rho1_max, &phi1_max, &rho2_max, &phi2_max);

    for (i = nrl; i <= nrh; i++) {
        for (j = ncl; j <= nch; j++) {
            output_image[i][j].r = input_img[i][j];
            output_image[i][j].g = input_img[i][j];
            output_image[i][j].b = input_img[i][j];
        }
    }

    double phi1_max_rad = M_PI * phi1_max / 180;
    double phi2_max_rad = M_PI * phi2_max / 180;
    for (i = nrl; i <= nrh; i++) {
        int j1 = (rho1_max - i * cos(phi1_max_rad)) / sin(phi1_max_rad);

        if (j1 >= ncl && j1 <= nrh) {
            output_image[i][j1].r = 255;
            output_image[i][j1].g = 0;
            output_image[i][j1].b = 0;
        }

        int j2 = (rho2_max - i * cos(phi2_max_rad)) / sin(phi2_max_rad);
        if (j2 >= ncl && j2 <= nrh) {
            output_image[i][j2].r = 255;
            output_image[i][j2].g = 0;
            output_image[i][j2].b = 0;
        }
    }

    SavePGM_bmatrix(hough_buildup_table, 0, diag, 0, 180, "data/output/route0_hough_accumulation_table_t" STR(WHITE_THRESHOLD) ".pgm");
    SavePPM_rgb8matrix(output_image, nrl, nrh, ncl, nch, "data/output/route0_with_line_detection_t" STR(WHITE_THRESHOLD) ".ppm");

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(binarized, nrl, nrh, ncl, nch);
    free_rgb8matrix(output_image, nrl, nrh, ncl, nch);
    free_bmatrix(hough_buildup_table, 0, diag, 0, 180);
    return EXIT_SUCCESS;
}