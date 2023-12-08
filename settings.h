// --- Tile ---
typedef struct {
    int cost;
} TileConf;
const TileConf tile_conf = {
    .cost = 5,
};

// --- Landmine ---
typedef struct {
    int cost;
    int bandwidth;
} LandmineConf;
const LandmineConf landmine_conf = {
    .cost = 69,
    .bandwidth = 1,
};

// --- Terrain ---
typedef struct {
    int hill_cost;
    int road_cost;
    int field_cost;
    int river_cost;
    int forest_cost;
} TerrainConf;
const TerrainConf terrain_conf = {
    .hill_cost = 5,
    .road_cost = 1,
    .field_cost = 3,
    .river_cost = 8,
    .forest_cost = 7,
};

// --- Available Settings ---
typedef struct {
    TileConf tile;
    LandmineConf landmine;
    TerrainConf terrain;
} Settings;
const Settings config = {
    .tile = tile_conf,
    .landmine = landmine_conf,
    .terrain = terrain_conf,
};
