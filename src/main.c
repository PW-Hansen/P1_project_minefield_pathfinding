#include <stdio.h>
#include <stdlib.h>
#include "kernel_density_estimation.h"
#include "terrain_map.h"

int main() {
  char file_path[1024];
  printf("Enter file path: ");
  (void) scanf(" %s", file_path);
  map_t terrain_map = map_from_ppm(file_path);

  for (int i = 0; i < terrain_map.width; i++) {
    for (int j = 0; j < terrain_map.height; j++) {
      printf("%1d", terrain_map.data[i][j]);
    }
    printf("\n");
  }

  free_map(terrain_map);
  printf("Thank you for using our software :)\n");

  kde_settings_t kde_settings = {5, 5, 1, 3, 1};

  int **map = malloc(sizeof(int *) * kde_settings.x_size);
  for (int i = 0; i < kde_settings.y_size; i++) {
      map[i] = malloc(sizeof(int) * kde_settings.y_size);
  }

  for (int x = 0; x < kde_settings.x_size; x++) {
      for (int y = 0; y < kde_settings.y_size; y++) {
          map[x][y] = 0;
      }
  }

  map[1][3] = 1;
  map[4][3] = 1;
  map[4][0] = 1;

  kernel_main(map, kde_settings);

  free(map);

  return 0;
}
