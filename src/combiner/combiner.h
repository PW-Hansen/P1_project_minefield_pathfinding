typedef enum {
    HILL = 1,
    ROAD = 2,
    FIELD = 3,
    RIVER = 4,
    FOREST = 5,
} TileType;

void combine_arrays(int **terrain_layout, double **kde_layout, double **cost_map);
