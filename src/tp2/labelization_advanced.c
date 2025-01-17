#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

#define STR_(X) #X
#define STR(X) STR_(X)

#define LOOKUP_TABLE_MAX_SIZE 1000

void* safe_malloc(const size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        perror("Error when allocating memory");
        exit(EXIT_FAILURE);
    }
    return p;
}

void init_lookup_table(int *lookup_table) {
    for (int i = 0; i < LOOKUP_TABLE_MAX_SIZE; i++) {
        lookup_table[i] = i;
    }
}

int imin(int a, int b) {
    return a < b ? a : b;
}

int imax(int a, int b) {
    return a > b ? a : b;
}

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    byte **input_img = LoadPGM_bmatrix("data/output/rice_binarized_avg_t110.pgm", &nrl, &nrh, &ncl, &nch);
    byte **output_img = bmatrix(nrl, nrh, ncl, nch);
    int **etiq_mat = imatrix(nrl, nrh, ncl, nch);

    int i, j;

    int *lookup_table = safe_malloc(LOOKUP_TABLE_MAX_SIZE * sizeof(int));
    init_lookup_table(lookup_table);

    int num_etiq = 1;
    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            if (input_img[i][j] == 0) {
                etiq_mat[i][j] = 0;
            } else if (input_img[i][j - 1] == input_img[i][j] && input_img[i - 1][j] != input_img[i][j]) {
                etiq_mat[i][j] = etiq_mat[i][j - 1];
            } else if (input_img[i - 1][j] == input_img[i][j] && input_img[i][j - 1] != input_img[i][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
            } else if (input_img[i][j - 1] != input_img[i][j] && input_img[i - 1][j] != input_img[i][j]) {
                etiq_mat[i][j] = num_etiq++;
            } else if (input_img[i][j - 1] == input_img[i][j] && input_img[i - 1][j] == input_img[i][j] && etiq_mat[i][j - 1] == etiq_mat[i - 1][j]) {
                etiq_mat[i][j] = etiq_mat[i - 1][j];
            } else if (input_img[i][j - 1] == input_img[i][j] && input_img[i - 1][j] == input_img[i][j] && etiq_mat[i][j - 1] != etiq_mat[i - 1][j]) {
                etiq_mat[i][j] = imin(lookup_table[etiq_mat[i - 1][j]], etiq_mat[i][j - 1]);
                lookup_table[etiq_mat[i][j]] = etiq_mat[i][j];
                lookup_table[etiq_mat[i][j - 1]] = etiq_mat[i][j];
                lookup_table[imax(lookup_table[etiq_mat[i - 1][j]], etiq_mat[i][j - 1])] = etiq_mat[i][j];
            }
        }
    }

    int effectiv_num_etiq = 1;
    for (i = 0; i < num_etiq; i++) {
        if (lookup_table[i] == i) {
            lookup_table[i] = effectiv_num_etiq++;
        } else {
            lookup_table[i] = lookup_table[lookup_table[i]];
        }
    }

    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            etiq_mat[i][j] = lookup_table[etiq_mat[i][j]];
        }
    }

    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            output_img[i][j] = etiq_mat[i][j] / (double) effectiv_num_etiq * 255;
        }
    }

    SavePGM_bmatrix(output_img, nrl, nrh, ncl, nch, "data/output/rice_labelized_advanced.pgm");

    free_bmatrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(output_img, nrl, nrh, ncl, nch);
    free_imatrix(etiq_mat, nrl, nrh, ncl, nch);

    free(lookup_table);
    return EXIT_SUCCESS;
}