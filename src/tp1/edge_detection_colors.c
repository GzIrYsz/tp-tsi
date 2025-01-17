#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nralloc.h"
#include "nrarith.h"
#include "nrio.h"

#define STR_(X) #X
#define STR(X) STR_(X)

#define EDGE_RED_THRESHOLD 35
#define EDGE_GREEN_THRESHOLD 35
#define EDGE_BLUE_THRESHOLD 35

int main(int argc, char **argv) {
    long nrl, nrh, ncl, nch;
    rgb8 **input_img = LoadPPM_rgb8matrix("data/input/kids.ppm", &nrl, &nrh, &ncl, &nch);
    byte **output_img_edges_detection_red = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_green = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_blue = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_rgb_1plane = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_rgb_2planes = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_rgb_3planes = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_rgb_hystheresis13 = bmatrix(nrl, nrh, ncl, nch);
    byte **output_img_edges_detection_rgb_hystheresis23 = bmatrix(nrl, nrh, ncl, nch);

    int i, j;

    int gradient_norm_red;
    byte edge_red;
    int gradient_norm_green;
    byte edge_green;
    int gradient_norm_blue;
    byte edge_blue;
    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            gradient_norm_red = (int) sqrt(pow((input_img[i - 1][j - 1].r * -1
                            + input_img[i][j - 1].r * 0
                            + input_img[i + 1][j - 1].r * 1
                            + input_img[i - 1][j].r * -2
                            + input_img[i][j].r * 0
                            + input_img[i + 1][j].r * 2
                            + input_img[i - 1][j + 1].r * -1
                            + input_img[i][j + 1].r * 0
                            + input_img[i + 1][j + 1].r * 1) / (double) 4, 2)
                            + pow((input_img[i - 1][j - 1].r * -1
                            + input_img[i][j - 1].r * -2
                            + input_img[i + 1][j - 1].r * -1
                            + input_img[i - 1][j].r * 0
                            + input_img[i][j].r * 0
                            + input_img[i + 1][j].r * 0
                            + input_img[i - 1][j + 1].r * 1
                            + input_img[i][j + 1].r * 2
                            + input_img[i + 1][j + 1].r * 1) / (double) 4, 2));

            gradient_norm_green = (int) sqrt(pow((input_img[i - 1][j - 1].g * -1
                            + input_img[i][j - 1].g * 0
                            + input_img[i + 1][j - 1].g * 1
                            + input_img[i - 1][j].g * -2
                            + input_img[i][j].g * 0
                            + input_img[i + 1][j].g * 2
                            + input_img[i - 1][j + 1].g * -1
                            + input_img[i][j + 1].g * 0
                            + input_img[i + 1][j + 1].g * 1) / (double) 4, 2)
                            + pow((input_img[i - 1][j - 1].g * -1
                            + input_img[i][j - 1].g * -2
                            + input_img[i + 1][j - 1].g * -1
                            + input_img[i - 1][j].g * 0
                            + input_img[i][j].g * 0
                            + input_img[i + 1][j].g * 0
                            + input_img[i - 1][j + 1].g * 1
                            + input_img[i][j + 1].g * 2
                            + input_img[i + 1][j + 1].g * 1) / (double) 4, 2));

            gradient_norm_blue = (int) sqrt(pow((input_img[i - 1][j - 1].b * -1
                            + input_img[i][j - 1].b * 0
                            + input_img[i + 1][j - 1].b * 1
                            + input_img[i - 1][j].b * -2
                            + input_img[i][j].b * 0
                            + input_img[i + 1][j].b * 2
                            + input_img[i - 1][j + 1].b * -1
                            + input_img[i][j + 1].b * 0
                            + input_img[i + 1][j + 1].b * 1) / (double) 4, 2)
                            + pow((input_img[i - 1][j - 1].b * -1
                            + input_img[i][j - 1].b * -2
                            + input_img[i + 1][j - 1].b * -1
                            + input_img[i - 1][j].b * 0
                            + input_img[i][j].b * 0
                            + input_img[i + 1][j].b * 0
                            + input_img[i - 1][j + 1].b * 1
                            + input_img[i][j + 1].b * 2
                            + input_img[i + 1][j + 1].b * 1) / (double) 4, 2));

            edge_red = gradient_norm_red > EDGE_RED_THRESHOLD ? 255 : 0;
            output_img_edges_detection_red[i][j] = edge_red;
            edge_green = gradient_norm_green > EDGE_GREEN_THRESHOLD ? 255 : 0;
            output_img_edges_detection_green[i][j] = edge_green;
            edge_blue = gradient_norm_blue > EDGE_BLUE_THRESHOLD ? 255 : 0;
            output_img_edges_detection_blue[i][j] = edge_blue;

            if (edge_red || edge_green || edge_blue) output_img_edges_detection_rgb_1plane[i][j] = 255;
            if ((edge_red && edge_green) || (edge_red && edge_blue) || (edge_green && edge_blue)) output_img_edges_detection_rgb_2planes[i][j] = 255;
            if (edge_red && edge_green && edge_blue) output_img_edges_detection_rgb_3planes[i][j] = 255;
        }
    }

    for (i = nrl + 1; i < nrh; i++) {
        for (j = ncl + 1; j < nch; j++) {
            output_img_edges_detection_rgb_hystheresis13[i][j] = output_img_edges_detection_rgb_3planes[i][j];
            if (output_img_edges_detection_rgb_1plane[i][j]
                 && (output_img_edges_detection_rgb_3planes[i - 1][j - 1]
                     || output_img_edges_detection_rgb_3planes[i - 1][j]
                     || output_img_edges_detection_rgb_3planes[i - 1][j + 1]
                     || output_img_edges_detection_rgb_3planes[i][j - 1]
                     || output_img_edges_detection_rgb_3planes[i][j + 1]
                     || output_img_edges_detection_rgb_3planes[i + 1][j - 1]
                     || output_img_edges_detection_rgb_3planes[i + 1][j]
                     || output_img_edges_detection_rgb_3planes[i + 1][j + 1])) output_img_edges_detection_rgb_hystheresis13[i][j] = output_img_edges_detection_rgb_1plane[i][j];

            output_img_edges_detection_rgb_hystheresis23[i][j] = output_img_edges_detection_rgb_3planes[i][j];
            if (output_img_edges_detection_rgb_2planes[i][j]
                 && (output_img_edges_detection_rgb_3planes[i - 1][j - 1]
                     || output_img_edges_detection_rgb_3planes[i - 1][j]
                     || output_img_edges_detection_rgb_3planes[i - 1][j + 1]
                     || output_img_edges_detection_rgb_3planes[i][j - 1]
                     || output_img_edges_detection_rgb_3planes[i][j + 1]
                     || output_img_edges_detection_rgb_3planes[i + 1][j - 1]
                     || output_img_edges_detection_rgb_3planes[i + 1][j]
                     || output_img_edges_detection_rgb_3planes[i + 1][j + 1])) output_img_edges_detection_rgb_hystheresis23[i][j] = output_img_edges_detection_rgb_2planes[i][j];
        }
    }

    SavePGM_bmatrix(output_img_edges_detection_red, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_red_t" STR(EDGE_RED_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_green, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_green_t" STR(EDGE_GREEN_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_blue, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_blue_t" STR(EDGE_BLUE_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_rgb_1plane, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_rgb_1plane_redt" STR(EDGE_RED_THRESHOLD) "_greent" STR(EDGE_GREEN_THRESHOLD) "_bluet" STR(EDGE_BLUE_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_rgb_2planes, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_rgb_2planes_redt" STR(EDGE_RED_THRESHOLD) "_greent" STR(EDGE_GREEN_THRESHOLD) "_bluet" STR(EDGE_BLUE_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_rgb_3planes, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_rgb_3planes_redt" STR(EDGE_RED_THRESHOLD) "_greent" STR(EDGE_GREEN_THRESHOLD) "_bluet" STR(EDGE_BLUE_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_rgb_hystheresis13, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_rgb_hystheresis13_redt" STR(EDGE_RED_THRESHOLD) "_greent" STR(EDGE_GREEN_THRESHOLD) "_bluet" STR(EDGE_BLUE_THRESHOLD) ".pgm");
    SavePGM_bmatrix(output_img_edges_detection_rgb_hystheresis23, nrl, nrh, ncl, nch, "data/output/kids_edges_detection_rgb_hystheresis23_redt" STR(EDGE_RED_THRESHOLD) "_greent" STR(EDGE_GREEN_THRESHOLD) "_bluet" STR(EDGE_BLUE_THRESHOLD) ".pgm");

    free_rgb8matrix(input_img, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_red, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_green, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_blue, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_rgb_1plane, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_rgb_2planes, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_rgb_3planes, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_rgb_hystheresis13, nrl, nrh, ncl, nch);
    free_bmatrix(output_img_edges_detection_rgb_hystheresis23, nrl, nrh, ncl, nch);
    return EXIT_SUCCESS;
}