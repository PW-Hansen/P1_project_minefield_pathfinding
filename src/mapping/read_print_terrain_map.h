#ifndef P1_PROJECT_MINEFIELD_PATHFINDING_READ_PRINT_TERRAIN_MAP_H
#define P1_PROJECT_MINEFIELD_PATHFINDING_READ_PRINT_TERRAIN_MAP_H

typedef struct {
    unsigned char red, green, blue;
} pixel_t;

typedef struct {
  int width;
  int height;
  int** data;
} map_t;

enum cell_types {
  CELL_INVALID = -1,
  CELL_ROAD,
  CELL_FIELD,
  CELL_FOREST,
  CELL_HILLS,
  CELL_RIVER
};

map_t map_from_ppm(const char* file_path);

#endif //P1_PROJECT_MINEFIELD_PATHFINDING_READ_PRINT_TERRAIN_MAP_H
