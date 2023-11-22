#include <stdio.h>
#include <stdlib.h>

#include "../settings.h"

int main() {
    printf("Tile base cost: %d\n", config.tile.cost);
    printf("Field tile cost: %d\n", config.terrain.field_cost);

    return EXIT_SUCCESS;
}
