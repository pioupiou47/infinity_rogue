
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "perso.h"



#define W_WIDTH 1216
#define W_HEIGHT 800


void SDL_ExitWithError(char *message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void SDL_TotalExit(char *message, SDL_Window* window, SDL_Renderer* renderer){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
}


void init_affichage(SDL_Window** window, SDL_Renderer** renderer)
{
    *window = NULL;
    *renderer = NULL;

    

    *window = SDL_CreateWindow("Infinity Rogue", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, 0);
    if(*window == NULL){
        SDL_ExitWithError("Creation fenetre echouee");
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
    if(*renderer == NULL){
        SDL_ExitWithError("Creation rendu echouee");
    }
}

void create_texture(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface** image, SDL_Texture** texture, char* fichier){
    *image = SDL_LoadBMP(fichier);
    
    if(*image == NULL){
        SDL_TotalExit("Impossible de charger l'image", window, renderer);
    }


    *texture = SDL_CreateTextureFromSurface(renderer, *image);
    if(*texture == NULL){
        SDL_TotalExit("Impossible de creer la texture", window, renderer);
    }
}

void load_texture(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rectangle){
    if(SDL_QueryTexture(texture, NULL, NULL, &(rectangle -> w), &(rectangle -> h)) != 0){
        SDL_TotalExit("Impossible de charger la texture", window, renderer);
    }
}

void draw_background(SDL_Window* window, SDL_Renderer* renderer, int** map, int width, int height){
    //0 pour de la terre
    //1 pour les portes
    //2 pour l'herbe

    SDL_Surface *herbe = NULL;
    SDL_Texture *texture_herbe = NULL;
    herbe = SDL_LoadBMP("src/herbe.bmp");
    
    create_texture(window, renderer, &herbe, &texture_herbe, "src/herbe.bmp");

    
    SDL_Rect rectangle_herbe;
    load_texture(window, renderer, texture_herbe, &rectangle_herbe);

    SDL_Surface *terre = NULL;
    SDL_Texture *texture_terre = NULL;
    create_texture(window, renderer, &terre, &texture_terre, "src/terre.bmp");
    
    SDL_Rect rectangle_terre;
    load_texture(window, renderer, texture_terre, &rectangle_terre);

    SDL_Surface *mur = NULL;
    SDL_Texture *texture_mur = NULL;
    create_texture(window, renderer, &mur, &texture_mur, "src/mur.bmp");
    
    SDL_Rect rectangle_mur;
    load_texture(window, renderer, texture_mur, &rectangle_mur);

    SDL_Surface *teleporteur = NULL;
    SDL_Texture *texture_teleporteur = NULL;
    create_texture(window, renderer, &teleporteur, &texture_teleporteur, "src/teleporteur.bmp");
    
    SDL_Rect rectangle_teleporteur;
    load_texture(window, renderer, texture_teleporteur, &rectangle_teleporteur);

    SDL_Surface *kaboom = NULL;
    SDL_Texture *texture_kaboom = NULL;
    create_texture(window, renderer, &kaboom, &texture_kaboom, "src/kaboom.bmp");
    
    SDL_Rect rectangle_kaboom;
    load_texture(window, renderer, texture_kaboom, &rectangle_kaboom);


    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(map[i][j] == 0 || map[i][j] == 1){
                rectangle_terre.x = j * 32;
                rectangle_terre.y = i * 32;
    
                if(SDL_RenderCopy(renderer, texture_terre, NULL, &rectangle_terre) != 0){
                    SDL_TotalExit("Impossible d'afficher la texture'", window, renderer);
                }
            }
            else if(map[i][j] == 2){
                rectangle_herbe.x = j * 32;
                rectangle_herbe.y = i * 32;
    
                if(SDL_RenderCopy(renderer, texture_herbe, NULL, &rectangle_herbe) != 0){
                    SDL_TotalExit("Impossible d'afficher la texture'", window, renderer);
                }
            }
            else if(map[i][j] == 3){
                rectangle_mur.x = j * 32;
                rectangle_mur.y = i * 32;
    
                if(SDL_RenderCopy(renderer, texture_mur, NULL, &rectangle_mur) != 0){
                    SDL_TotalExit("Impossible d'afficher la texture'", window, renderer);
                }
            }    
            else if(map[i][j] == 4){
                rectangle_teleporteur.x = j * 32;
                rectangle_teleporteur.y = i * 32;
    
                if(SDL_RenderCopy(renderer, texture_teleporteur, NULL, &rectangle_teleporteur) != 0){
                    SDL_TotalExit("Impossible d'afficher la texture'", window, renderer);
                }
            } 
            else if(map[i][j] == 5){
                rectangle_kaboom.x = j * 32;
                rectangle_kaboom.y = i * 32;
    
                if(SDL_RenderCopy(renderer, texture_kaboom, NULL, &rectangle_kaboom) != 0){
                    SDL_TotalExit("Impossible d'afficher la texture'", window, renderer);
                }
            }         
        }
    } 

}

void draw_perso(SDL_Window* window, SDL_Renderer* renderer, personnage* perso){
    //Valeur de direction par rapport aux animation
    //1 back
    //3 front

    SDL_Surface *sprite = NULL;
    SDL_Texture *texture_perso = NULL;

    char* nom_sprite = NULL ;
    nom_sprite = malloc (sizeof(char) * 100);
    for(int i = 0; i<100; i++){
        nom_sprite[i] = '\0';
    }

    strcat(nom_sprite, "src/perso_");
    if(perso -> moving){
        strcat(nom_sprite, "walk_");
        if(perso -> direction == 1){
            strcat(nom_sprite, "back_");
        }else{
            strcat(nom_sprite, "front_");
        }

        char* tmp = malloc(sizeof(char) * 2);
        tmp[0] = '1' + perso -> frame;
        tmp[1] = '\0';
        strcat(nom_sprite, tmp);
        free(tmp);

    }else{
        strcat(nom_sprite, "idle_");
        if(perso -> direction == 1){
            strcat(nom_sprite, "back");
        }else{
            strcat(nom_sprite, "front");
        }
    }

    strcat(nom_sprite, ".bmp\0");
    
    
    sprite = SDL_LoadBMP(nom_sprite);

    create_texture(window, renderer, &sprite, &texture_perso, nom_sprite);

    
    SDL_Rect rectangle;
    load_texture(window, renderer, texture_perso, &rectangle);

    //Les -50 sont là pour centrer le sprite par rapport aux coordonnées
    rectangle.x = perso -> coo.x-50;
    rectangle.y = perso -> coo.y-50;

    if(SDL_RenderCopy(renderer, texture_perso, NULL, &rectangle) != 0){
        SDL_TotalExit("Impossible d'afficher la texture'", window, renderer);
    }

    free(nom_sprite);
}