#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_img = LoadPGM_bmatrix("data/output/rice_binarized_avg_t110.pgm", &nrl, &nrh, &ncl, &nch);
    byte **output_img = bmatrix(nrl, nrh, ncl, nch);
    int **etiq_mat = imatrix(nrl, nrh, ncl, nch);


    int etiq = 1;

    int i, j;
    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            if (input_img[i][j] == 0) {
                etiq_mat[i][j] = 0;
            } else if (input_img[i][j - 1] == input_img[i][j] && input_img[i - 1][j] != input_img[i][j]) {
                etiq_mat[i][j] = etiq_mat[i][j - 1];
            } else if (input_img[i - 1][j] == input_img[i][j] && input_img[i][j - 1] != input_img[i][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
            } else if (input_img[i][j - 1] != input_img[i][j] && input_img[i - 1][j] != input_img[i][j]) {
                etiq_mat[i][j] = etiq++;
            } else if (input_img[i][j - 1] == input_img[i][j] && input_img[i - 1][j] == input_img[i][j] && etiq_mat[i][j - 1] == etiq_mat[i - 1][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
            } else if (input_img[i][j - 1] == input_img[i][j] && input_img[i - 1][j] == input_img[i][j] && etiq_mat[i][j - 1] != etiq_mat[i - 1][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
                for (int k = nrl + 1; k < i; k++) {
                    for (int l = ncl + 1; l < j; l++) {
                        if (etiq_mat[k][l] == etiq_mat[i][j - 1]) {
                            etiq_mat[k][l] = etiq_mat[i - 1][j];
                        }
                    }
                }
            }
        }
    }

    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            output_img[i][j] = etiq_mat[i][j] / (double) etiq * 255;
        }
    }

    SavePGM_bmatrix(output_img, nrl, nrh, ncl, nch, "data/output/rice_labelized_intuitive.pgm");

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(output_img, nrl, nrh, ncl, nch);
    free_imatrix(etiq_mat, nrl, nrh, ncl, nch);
    return EXIT_SUCCESS;
}