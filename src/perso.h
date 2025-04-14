#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

personnage* crea_perso(float x, float y);
void deplacement(personnage* perso, int direction, bool chgt_direction);
void incr_frame_anim(personnage* perso);
void rein_anim(personnage* perso);