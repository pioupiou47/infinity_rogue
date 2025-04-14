/*
    GNU/Linux et MacOS
        > gcc src/*.c -o bin/prog $(sdl2-config --cflags --libs)
    Windows
        > gcc src/*.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2

    Pour lancer :
        > .\bin\prog.exe
*/
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "affichage.h"
#include "map.h"

//Temps entre chaque fps (en ms)
#define FPS_LIMIT 33


void init_main_sdl(SDL_Window** window, SDL_Renderer** renderer){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("ERREUR : Initialisation SDL > %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    init_affichage(window, renderer);
}

void SDL_limit_FPS(unsigned int limit){
    unsigned int ticks = SDL_GetTicks();

    if(limit < ticks){
        return;
    }else if(limit > ticks + FPS_LIMIT){
        SDL_Delay(FPS_LIMIT);
    }else{
        SDL_Delay(limit - ticks);
    }
}


void collision_porte(personnage* perso, chunk** chu, dict* d){
    float x = perso -> coo.x;
    float y = perso -> coo.y;

    if(x < 0 && (*chu) -> portes_map[0] != NULL){
        *chu = move_to((*chu) -> portes_map[0], d);
        perso -> coo.x = W_WIDTH;
    }
    else if(y < 0 && (*chu) -> portes_map[1] != NULL){
        *chu = move_to((*chu) -> portes_map[1] , d);
        perso -> coo.y = W_HEIGHT;
    }
    else if(x > W_WIDTH && (*chu) -> portes_map[2] != NULL){
        *chu = move_to((*chu) -> portes_map[2], d);
        perso -> coo.x = 0;
    }
    else if(y > W_HEIGHT && (*chu) -> portes_map[3] != NULL){
        *chu = move_to((*chu) -> portes_map[3], d);
        perso -> coo.y = 0;
    }
}

chunk* collision_teleporteur(personnage* perso, chunk* chu, dict* d, int* last_action){
    int x = perso -> coo.x / W_WIDTH * C_WIDTH;
    int y = perso -> coo.y / W_HEIGHT * C_HEIGHT;

    //Test collision avec les murs
    if(x >= 0 && x < C_WIDTH && y >= 0 && y < C_HEIGHT && chu -> map[y][x] == 4){
        *last_action = SDL_GetTicks();

        chu = move_to(d_find(d, chu -> coo_obj_tel), d);
        bool bon = false;
        for(int i = 0; i<C_HEIGHT; i++){
            for(int j = 0; j<C_WIDTH; j++){
                if(chu -> map[i][j] == 4){
                    perso->coo.x = j * W_WIDTH/ C_WIDTH ;
                    perso->coo.y = i * W_HEIGHT/ C_HEIGHT ;
                    bon = true;
                    break;
                }
            }
            if(bon)
            break;
        }
    }
    return chu;
}

void collision_kaboom(personnage* perso, chunk* chu, dict* d, int* last_action){
    
    int x = perso -> coo.x / W_WIDTH * C_WIDTH;
    int y = perso -> coo.y / W_HEIGHT * C_HEIGHT;

    //Test collision avec les murs
    if(x >= 0 && x < C_WIDTH && y >= 0 && y < C_HEIGHT && chu -> map[y][x] == 5){
        *last_action = SDL_GetTicks();
        kaboom_chunk(chu, d);
    }
}

void collision(personnage* perso, chunk* chu, bool* dep){
    int x = perso -> coo.x / W_WIDTH * C_WIDTH;
    int y = perso -> coo.y / W_HEIGHT * C_HEIGHT;

    //Test collision avec les murs
    if(x >= 0 && x < C_WIDTH && y >= 0 && y < C_HEIGHT && chu -> map[y][x] == 3){
        for(int i = 0; i<4; i++){
            if(dep[i] == true){
                deplacement(perso, (i+2)%4, false);
            }
        }
    }
}


int main(int argc, char** argv)
{
    srand(time(NULL)) ;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    init_main_sdl(&window, &renderer);
       

    dict* d = dict_create();
    chunk* chunk_0 = create_map(0,0,d, false, -1, -1);
    creation_porte(chunk_0, d);


    personnage* perso = crea_perso(600,400);
    


    draw_background(window, renderer, chunk_0 -> map, chunk_0 -> width, chunk_0 -> heigth);
    SDL_bool pgr_launched = SDL_TRUE;
    unsigned int limit_fps = SDL_GetTicks() + FPS_LIMIT;

    unsigned int last_action = SDL_GetTicks();

    bool* banque_directions = malloc(sizeof(bool) * 4);
    for(int i = 0; i<4; i++){
        banque_directions[i] = false;
    }

    while(pgr_launched){
        //Gère les fps
        SDL_limit_FPS(limit_fps);
        limit_fps = SDL_GetTicks() + FPS_LIMIT;
        incr_frame_anim(perso);

        SDL_Event event;
        while(SDL_PollEvent(&event) == 1){
            if(event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_q)
                {
                    banque_directions[0] = true;
                }
                else if (event.key.keysym.sym == SDLK_z)
                {
                    banque_directions[1] = true;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    banque_directions[2] = true;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    banque_directions[3] = true;
                } 
            }

            else if(event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_q)
                {
                    banque_directions[0] = false;
                }
                else if (event.key.keysym.sym == SDLK_z)
                {
                    banque_directions[1] = false;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    banque_directions[2] = false;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    banque_directions[3] = false;
                } 
            }
            
            else if(event.type == SDL_QUIT){
                pgr_launched = SDL_FALSE;
                break;
            }            
        }
            
        bool bon = false;
        for(int i = 0; i<4; i++){
            if(banque_directions[i] == true){
                bon = true;
                deplacement(perso, i, true);
            }
        }
        collision(perso, chunk_0, banque_directions);
        collision_porte(perso, &chunk_0, d);
        if(last_action + 20 * FPS_LIMIT < SDL_GetTicks()){
            chunk_0 = collision_teleporteur(perso, chunk_0, d, &last_action);
            collision_kaboom(perso, chunk_0, d, &last_action);
        }

        if(bon == false){
            rein_anim(perso);
        }

        //affichages
        SDL_RenderClear(renderer);
        draw_background(window, renderer, chunk_0 -> map, chunk_0 -> width, chunk_0 -> heigth);
        draw_perso(window, renderer, perso);
        SDL_RenderPresent(renderer); 
              
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
