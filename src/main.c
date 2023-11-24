#include <stdio.h>
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

  return 0;
}
