#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SPEED 6
#define DELTA_FRAME_ANIM 4

struct coordonnes_f { float x; float y ;} ;
typedef struct coordonnes_f coor_f ;

struct personnages {
    coor_f coo;

    bool moving;
    int direction;

    int frame;
    int frame_reel;
    int frame_limit;
};
typedef struct personnages personnage ;

personnage* crea_perso(float x, float y){
    personnage* p = malloc(sizeof(personnage));

    coor_f coo;
    coo.x = x;
    coo.y = y;
    p -> coo = coo;

    p -> moving = false;

    p -> direction = 1;
    p -> frame = 0;
    p -> frame_limit = 1;

    return p;
}

void incr_frame_anim(personnage* perso){
    perso -> frame_reel++;
    perso -> frame_reel =  perso -> frame_reel % (perso -> frame_limit * DELTA_FRAME_ANIM );
    perso -> frame = perso -> frame_reel / DELTA_FRAME_ANIM;
}
void rein_anim(personnage* perso){
    perso -> frame = 0;
    perso -> moving = false;
    perso -> frame_limit = 1;
} 

void deplacement(personnage* perso, int direction, bool chgt_direction){
    //0 pour à gauche       1
    //1 pour en haut        |
    //2 pour à droite   0---|---2
    //3 pour en bas         |
    //                      3
    perso -> moving = true;
    perso -> frame_limit = 6;

    switch (direction)
    {
    case 0:
        perso -> coo.x -= SPEED; 
        break;
    
    case 1:
        perso -> coo.y -= SPEED; 
        if(chgt_direction)
            perso -> direction = 1;
        break;

    case 2:
        perso -> coo.x += SPEED; 
        break;

    case 3:
        perso -> coo.y += SPEED;
        if(chgt_direction)
            perso -> direction = 3;
        break;
    
    default:
        break;
    }

}