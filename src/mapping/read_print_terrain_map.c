#include <stdio.h>
#include <stdlib.h>
#include "read_print_terrain_map.h"

#define MAX_WIDTH 1000
#define MAX_HEIGHT 1000

//we create the struct of "pixel"
typedef struct {
    unsigned char red, green, blue;
} Pixel;

void print_result_array() {

};

int read_print_terrainmap() {
    // Open a file for writing
    FILE *outputFile = freopen("output.txt", "w", stdout);

    // Check if the file was opened successfully
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    //open the map-file in bit read mode
    FILE *file = fopen("testmap.ppm", "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read and skip the P6 header
    char header[3];
    fgets(header, sizeof(header), file);

    // we check if the header is the right type of P6
    if (header[0] != 'P' || header[1] != '6') {
        printf("Invalid PPM file format.\n");
        fclose(file);
        return 1;
    }

    // read the width, height, and maximum color value
    int width, height, maxColor;
    fscanf(file, "%d %d %d", &width, &height, &maxColor);

    // check if the color is 8 bit
    if (width >= MAX_WIDTH && height>= MAX_HEIGHT && maxColor != 255) {
        printf("Unsupported size or color depth.\n");
        fclose(file);
        return 1;
    }

    // skip the newline character after the header
    fgetc(file);

    // read pixel values into a 2D array
    Pixel terrainMap[height][width];
    fread(terrainMap, sizeof(Pixel), width * height, file);

    // close the file
    fclose(file);

    // process the terrainMap array based on the color information
    int resultArray[width][height];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (terrainMap[i][j].red == 120 && terrainMap[i][j].green == 120 && terrainMap[i][j].blue == 120) {
                resultArray[i][j] = 1;
            } else if (terrainMap[i][j].red == 210 && terrainMap[i][j].green == 230 && terrainMap[i][j].blue == 130) {
                resultArray[i][j] = 2;
            } else if (terrainMap[i][j].red == 50 && terrainMap[i][j].green == 100 && terrainMap[i][j].blue == 60) {
                resultArray[i][j] = 3;
            } else if (terrainMap[i][j].red == 170 && terrainMap[i][j].green == 140 && terrainMap[i][j].blue == 70) {
                resultArray[i][j] = 4;
            } else if (terrainMap[i][j].red == 50 && terrainMap[i][j].green == 70 && terrainMap[i][j].blue == 100) {
                resultArray[i][j] = 5;
            }
        }
    }

    // print the resultArray
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            printf("%2d ", resultArray[i][j]);
        }
        printf("\n");
    }
    // close the output file
    fclose(outputFile);

    return 0;
}
