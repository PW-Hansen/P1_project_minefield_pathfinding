#ifndef P1_PROJECT_MINEFIELD_PATHFINDING_READ_PRINT_TERRAIN_MAP_H
#define P1_PROJECT_MINEFIELD_PATHFINDING_READ_PRINT_TERRAIN_MAP_H

typedef struct {
    unsigned char red, green, blue;
} pixel_t;

typedef struct {
    int width;
    int height;
    int **data;
} map_t;

enum cell_types {
    CELL_INVALID = -1,
    CELL_ROAD,
    CELL_FIELD,
    CELL_FOREST,
    CELL_HILLS,
    CELL_RIVER
};

void free_map(map_t map);

map_t map_from_ppm(const char *file_path);

int hotspot_pos_from_ppm(const char *file_path, mine_tuple_t* hotspot_map);

#endif //P1_PROJECT_MINEFIELD_PATHFINDING_READ_PRINT_TERRAIN_MAP_H
