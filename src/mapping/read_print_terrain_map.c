#include <stdio.h>
#include <stdlib.h>

#define MAX_WIDTH 1000
#define MAX_HEIGHT 1000

//we create the struct of "pixel"
typedef struct {
    unsigned char red, green, blue;
} Pixel;

int pixel_equal(Pixel p1, Pixel p2) {
    return p1.red == p2.red && p1.green == p2.green && p1.blue == p2.blue;
}

enum cell_types {invalid = -1, road, field, forest, hills, river};

enum cell_types get_type_of_pixel(Pixel p) {
    Pixel color_types[] = {{120, 120, 120},
                           {210, 230, 130},
                           {50, 100, 60},
                           {170, 140, 70},
                           {50, 70, 100}};
    for (int i = 0; i < 5; i++) {
        if (pixel_equal(p, color_types[i])) {
            return (enum cell_types)i;
        }
    }
    return invalid;
}

int main() {
    // open the map-file in bit read mode
    FILE *file = fopen("C:\\Users\\matia\\CLionProjects\\project_map\\testmap.ppm", "rb");
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

    // check if the color is 8 bit and image is proper size
    if (width >= MAX_WIDTH && height >= MAX_HEIGHT && maxColor != 255) {
        printf("Unsupported size or color depth.\n");
        fclose(file);
        return 1;
    }

    // skip the newline character after the header
    fgetc(file);

    // read pixel values into a 2D array
    Pixel terrain_map[height][width];
    fread(terrain_map, sizeof(Pixel), width * height, file);

    // close the file
    fclose(file);

    // process the terrain_map array based on the color information
    int result_array[width][height];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            result_array[i][j] = get_type_of_pixel(terrain_map[i][j]);
        }
    }

    // print the result_array
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            printf("%2d ", result_array[i][j]);
        }
        printf("\n");
    }

    return 0;
}
