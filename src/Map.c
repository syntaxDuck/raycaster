#include "Map.h"

// Function to print the map values to the console
void printMap(Map map)
{
    printf("Walls:\n");
    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            printf("%d ", map.walls[i][j]); // Print each value in the row
        }
        printf("\n"); // Newline after each row
    }

    printf("Ceil:\n");
    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            printf("%d ", map.ceil[i][j]); // Print each value in the row
        }
        printf("\n"); // Newline after each row
    }

    printf("Floor:\n");
    for (int i = 0; i < map.height; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            printf("%d ", map.floor[i][j]); // Print each value in the row
        }
        printf("\n"); // Newline after each row
    }
}

Map loadMap(char *filename)
{
    Map map;
    map.unit_size = MAP_UNIT_SIZE;
    loadMapGridFromFile(filename, &map);
    return map;
}

void loadMapGridFromFile(const char *filename, Map *map)
{
    // Open the file
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to open map file: %s\n", filename);
        exit(1);
    }

    // Read the width and height of the map
    fscanf(file, "%d %d", &map->width, &map->height);

    // Allocate memory for the map
    Uint8 **walls = malloc(map->height * sizeof(Uint8 *));
    Uint8 **ceil = malloc(map->height * sizeof(Uint8 *));
    Uint8 **floor = malloc(map->height * sizeof(Uint8 *));
    if (!walls || !ceil || !floor)
    {
        fprintf(stderr, "Failed to allocate memory for map\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < map->height; i++)
    {
        walls[i] = malloc(map->width * sizeof(Uint8));
        if (!walls[i])
        {
            fprintf(stderr, "Failed to allocate memory for walls row\n");
            fclose(file);
            return;
        }

        // Read each value into the map
        for (int j = 0; j < map->width; j++)
        {
            if (fscanf(file, "%hhu", &walls[i][j]) != 1)
            {
                fprintf(stderr,
                        "Failed to read value for walls[%d][%d]\n",
                        i, j);
                fclose(file);
                return;
            }
        }
    }

    for (int i = 0; i < map->height; i++)
    {
        ceil[i] = malloc(map->width * sizeof(Uint8));
        if (!ceil[i])
        {
            fprintf(stderr, "Failed to allocate memory for ceil row\n");
            fclose(file);
            return;
        }

        // Read each value into the map
        for (int j = 0; j < map->width; j++)
        {
            if (fscanf(file, "%hhu", &ceil[i][j]) != 1)
            {
                fprintf(stderr,
                        "Failed to read value for ceil[%d][%d]\n",
                        i, j);
                fclose(file);
                return;
                ;
            }
        }
    }

    for (int i = 0; i < map->height; i++)
    {
        floor[i] = malloc(map->width * sizeof(Uint8));
        if (!floor[i])
        {
            fprintf(stderr, "Failed to allocate memory for floor row\n");
            fclose(file);
            return;
            ;
        }

        // Read each value into the map
        for (int j = 0; j < map->width; j++)
        {
            if (fscanf(file, "%hhu", &floor[i][j]) != 1)
            {
                fprintf(stderr,
                        "Failed to read value for floor[%d][%d]\n",
                        i, j);
                fclose(file);
                return;
            }
        }
    }
    map->walls = walls;
    map->ceil = ceil;
    map->floor = floor;
    fclose(file);
}

// Function to free the dynamically allocated map memory
void freeMap(Map map)
{
    for (int i = 0; i < map.height; i++)
    {
        free(map.walls[i]);
        free(map.ceil[i]);
        free(map.floor[i]);
    }
    free(map.walls);
    free(map.ceil);
    free(map.floor);
}