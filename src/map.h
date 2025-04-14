#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define C_WIDTH 38
#define C_HEIGHT 25


struct coordonnes { int x; int y ;} ;
typedef struct coordonnes coor ;

struct chunks {
    int** map;
    coor coo ;
    struct chunks** portes_map;
    int nb_porte;

    int heigth;
    int width;

    coor coo_obj_tel;
};
typedef struct chunks chunk ;

struct l_chaine { 
    struct chunks* c;
    struct l_chaine* next;
};
typedef struct l_chaine l_chaine ;

struct dict { 
    l_chaine** values;
    int len;
};
typedef struct dict dict ;

int hash(coor coo, int len);
dict* dict_create();
int d_len(dict* d);
bool d_mem(dict* d, coor co);
chunk* d_find(dict* d, coor co);
void d_add(dict* d, chunk* c);
void d_remove(dict* d, coor coo);


void actualisation_map(chunk* chu);
void delete_chunk(chunk* chu, dict* d);
void kaboom_chunk(chunk* chu, dict* d);
int dist(int x1, int y1, int x2, int y2);
float dist_f(int x1, int y1, int x2, int y2);
void herbe(chunk* chu, float pourcent);
chunk* create_map(int x, int y, dict* d, bool teleporteur, int x_tel, int y_tel);
void creation_porte(chunk* chu, dict* d);
chunk* move_to(chunk* c, dict* d);
