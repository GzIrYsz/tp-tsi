#include <limits.h>
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
    byte **binarized = bmatrix(nrl, nrh, ncl, nch);
    int **etiq_mat = imatrix(nrl, nrh, ncl, nch);
    rgb8 **output_img = rgb8matrix(nrl, nrh, ncl, nch);

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

    int etiq = 1;
    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            if (binarized[i][j] == 0) {
                etiq_mat[i][j] = 0;
            } else if (binarized[i][j - 1] == binarized[i][j] && binarized[i - 1][j] != binarized[i][j]) {
                etiq_mat[i][j] = etiq_mat[i][j - 1];
            } else if (binarized[i - 1][j] == binarized[i][j] && binarized[i][j - 1] != binarized[i][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
            } else if (binarized[i][j - 1] != binarized[i][j] && binarized[i - 1][j] != binarized[i][j]) {
                etiq_mat[i][j] = etiq++;
            } else if (binarized[i][j - 1] == binarized[i][j] && binarized[i - 1][j] == binarized[i][j] && etiq_mat[i][j - 1] == etiq_mat[i - 1][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
            } else if (binarized[i][j - 1] == binarized[i][j] && binarized[i - 1][j] == binarized[i][j] && etiq_mat[i][j - 1] != etiq_mat[i - 1][j]) {
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

    int c_etiq;
    printf("Il y a %d étiquettes (certaines peuvent cependant être inexistantes), laquelle isoler : ", etiq);
    scanf("%d", &c_etiq);

    int sum_x = 0;
    int num_x = 0;
    int sum_y = 0;
    int num_y = 0;
    int min_x = INT_MAX;
    int max_x = INT_MIN;
    int min_y = INT_MAX;
    int max_y = INT_MIN;
    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            output_img[i][j].r = input_img[i][j];
            output_img[i][j].g = input_img[i][j];
            output_img[i][j].b = input_img[i][j];

            if (etiq_mat[i][j] == c_etiq) {
                sum_x += j;
                num_x++;
                sum_y += i;
                num_y++;

                if (j < min_x) min_x = j;
                if (j > max_x) max_x = j;
                if (i < min_y) min_y = i;
                if (i > max_y) max_y = i;
            }
        }
    }
    int x = sum_x / num_x;
    int y = sum_y / num_y;
    output_img[y - 1][x - 1].r = 255;
    output_img[y - 1][x - 1].g = 0;
    output_img[y - 1][x - 1].b = 0;
    output_img[y - 1][x].r = 255;
    output_img[y - 1][x].g = 0;
    output_img[y - 1][x].b = 0;
    output_img[y - 1][x + 1].r = 255;
    output_img[y - 1][x + 1].g = 0;
    output_img[y - 1][x + 1].b = 0;
    output_img[y][x - 1].r = 255;
    output_img[y][x - 1].g = 0;
    output_img[y][x - 1].b = 0;
    output_img[y][x].r = 255;
    output_img[y][x].g = 0;
    output_img[y][x].b = 0;
    output_img[y][x + 1].r = 255;
    output_img[y][x + 1].g = 0;
    output_img[y][x + 1].b = 0;
    output_img[y + 1][x - 1].r = 255;
    output_img[y + 1][x - 1].g = 0;
    output_img[y + 1][x - 1].b = 0;
    output_img[y + 1][x].r = 255;
    output_img[y + 1][x].g = 0;
    output_img[y + 1][x].b = 0;
    output_img[y + 1][x + 1].r = 255;
    output_img[y + 1][x + 1].g = 0;
    output_img[y + 1][x + 1].b = 0;

    for (i = min_y; i <= max_y; i++) {
        output_img[i][min_x].r = 255;
        output_img[i][min_x].g = 0;
        output_img[i][min_x].b = 0;

        output_img[i][max_x].r = 255;
        output_img[i][max_x].g = 0;
        output_img[i][max_x].b = 0;
    }
    for (j = min_x; j <= max_x; j++) {
        output_img[min_y][j].r = 255;
        output_img[min_y][j].g = 0;
        output_img[min_y][j].b = 0;

        output_img[max_y][j].r = 255;
        output_img[max_y][j].g = 0;
        output_img[max_y][j].b = 0;
    }

    char *output_path = (char*) malloc(sizeof(char));
    sprintf(output_path, "data/output/rice_labelized_intuitive_with_interest_region_num%d.pgm", c_etiq);
    SavePPM_rgb8matrix(output_img, nrl, nrh, ncl, nch, output_path);

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(binarized, nrl, nrh, ncl, nch);
    free_imatrix(etiq_mat, nrl, nrh, ncl, nch);
    free_rgb8matrix(output_img, nrl, nrh, ncl, nch);
    return EXIT_SUCCESS;
}