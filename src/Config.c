#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static int create_config()
{
    FILE *file = fopen(CONFIG_PATH, "w+");
    if (!file)
    {
        fprintf(stderr, "Could not create new window_config file: %s\n", CONFIG_PATH);
        return -1;
    }
    fprintf(file, "win_title=%s\n", DEFAULT_WIN_TITLE);
    fprintf(file, "win_width=%d\n", DEFAULT_WIN_WIDTH);
    fprintf(file, "win_height=%d\n", DEFAULT_WIN_HEIGHT);
    fprintf(file, "win_max_fps=%d\n", DEFAULT_WIN_MAX_FPS);

    fclose(file);
    return 0;
}

static FILE *open_config()
{
    FILE *file = fopen(CONFIG_PATH, "r+");
    if (!file)
    {
        fprintf(stderr, "Could not open window_config file: %s\n", CONFIG_PATH);
        return NULL;
    }
    return file;
}

// TODO: Create error handeling when window_config file is formatted wrong
static Config *read_config()
{
    FILE *file = open_config();
    Config *window_config = malloc(sizeof(Config));
    window_config->ptr_window_config = malloc(sizeof(WindowConfig));

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (key && value)
        {
            if (strcmp(key, "win_title") == 0)
                window_config->ptr_window_config->title = strdup(value);
            else if (strcmp(key, "win_width") == 0)
                window_config->ptr_window_config->width = atoi(value);
            else if (strcmp(key, "win_height") == 0)
                window_config->ptr_window_config->height = atoi(value);
            else if (strcmp(key, "win_max_fps") == 0)
                window_config->ptr_window_config->max_fps = atoi(value);
        }
    }
    return window_config;
}

Config *init_config()
{
    FILE *file = open_config();
    if (!file)
    {
        create_config(file);
        if (!file)
            return NULL;
    }

    Config *window_config = read_config(file);
    fclose(file);
    return window_config;
}

void print_config()
{
    FILE *file = open_config();
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
}
