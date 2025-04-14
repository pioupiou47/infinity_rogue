#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "perso.h"

#define W_WIDTH 1216
#define W_HEIGHT 800

void SDL_TotalExit(char *message, SDL_Window* window, SDL_Renderer* renderer);

void init_affichage(SDL_Window** window, SDL_Renderer** renderer);
void draw_background(SDL_Window* window, SDL_Renderer* renderer, int** map, int width, int height);
void draw_perso(SDL_Window* window, SDL_Renderer* renderer, personnage* perso);