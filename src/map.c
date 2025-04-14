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



int hash(coor coo, int len){
    return (((coo.x * 28 + coo.y * 7) % len) + len) % len;
}

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



dict* dict_create(){
    dict* d = malloc(sizeof(dict));
    l_chaine** v = malloc(sizeof(l_chaine*) * 10);
    for(int i = 0; i<10; i++){
        v[i] = NULL;
    }

    d -> values = v;
    d -> len = 10;
    return d;
}

int d_len(dict* d){
    return d -> len;
}

bool d_mem(dict* d, coor co){
    bool pre = false;
    int i = hash(co, d_len(d));
    
    l_chaine* ch = d -> values[i];
    
    
    while(ch != NULL){
        if(ch == NULL){
            break;
        }
        if(ch -> c -> coo.x == co.x && ch -> c -> coo.y == co.y){
            pre = true;
            break;
        }
        
        ch = ch -> next;
    }
    return pre;
}


chunk* d_find(dict* d, coor co){
    chunk* chun ;
    int i = hash(co, d_len(d));
    
    l_chaine* ch = d -> values[i];
    
    while(ch != NULL){
        if(ch -> c -> coo.x == co.x && ch -> c -> coo.y == co.y){
            
            return ch -> c;
        }
        
        ch = ch -> next;
    }
    return chun;
}

void d_add(dict* d, chunk* c){
    int i = hash(c -> coo, d_len(d));

    l_chaine* l = malloc(sizeof(l_chaine));
    l -> c = c;
    l -> next = d -> values[i];

    d -> values[i] = l;
}

void d_remove(dict* d, coor coo){
    int i = hash(coo, d_len(d));
    
    l_chaine* ch = d -> values[i];
    if(ch != NULL){
        if(ch -> c -> coo.x == coo.x && ch -> c -> coo.y == coo.y){
            l_chaine* tmp = ch -> next;
            free(tmp);
            d -> values[i] = NULL;
        }
        
        while(ch -> next != NULL){
        if(ch -> next -> c -> coo.x == coo.x && ch -> next -> c -> coo.y == coo.y){
            l_chaine* tmp = ch -> next;
            ch -> next = ch -> next -> next;
            
            free(tmp);
            break;
        }
        ch = ch -> next;
        }
    }
    

}



void actualisation_map(chunk* chu){
    if(chu -> width < 5 || chu -> heigth < 5){
        printf("fonction : actualisation_map -> Erreur, la map est trop petite");
    }

    if(chu -> portes_map[2] != NULL){
        //droite
        for(int i = C_HEIGHT/2-2; i<C_HEIGHT/2+3; i++){
            chu -> map[i][C_WIDTH-1] = 1;
        }
    }
    if(chu -> portes_map[0] != NULL){
        //gauche
        for(int i = C_HEIGHT/2-2; i<C_HEIGHT/2+3; i++){
            chu -> map[i][0] = 1;
        }
    }
    if(chu -> portes_map[1] != NULL){
        //haut
        for(int i = C_WIDTH/2-2; i<C_WIDTH/2+3; i++){
            chu -> map[0][i] = 1;
        }
    }
    if(chu -> portes_map[3] != NULL){
        //bas
        for(int i = C_WIDTH/2-2; i<C_WIDTH/2+3; i++){
            chu -> map[C_HEIGHT-1][i] = 1;
        }
    }
}



int dist(int x1, int y1, int x2, int y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

float dist_f(int x1, int y1, int x2, int y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

void herbe(chunk* chu, float pourcent){
    int x1 = rand()% (chu -> width);
    int y1 = rand()% (chu -> heigth);
    int x2 = rand()% (chu -> width);
    int y2 = rand()% (chu -> heigth);

    int obj = (chu -> width - 2) * (chu -> heigth - 2) * pourcent / 100;
    int act = 0;

    int i = rand()% (chu -> heigth);
    int j = rand()% (chu -> width);

    int d_diag = dist_f(0,0,chu -> width, chu -> heigth);
    

    while(act < obj){
        int d1 = dist_f(x1,y1, j,i);
        int d2 = dist_f(x2,y2, j,i);

        if(chu -> map[i][j] == 0 && rand()%1000 < pow(((1 - (d1 + 0.0)/d_diag)) * 10, 2)*2.5 +  pow((1 - (d2 + 0.0)/d_diag) * 10, 2)*2.5 ){
            chu -> map[i][j] = 2;
            act ++;
        }

        j++;
        if(j >= (chu -> width)){
            j = 0;
            i++;
        }
        i = i%(chu -> heigth);
    }
}

void ajout_teleporteur(chunk* chu, coor coo){
    bool bon = false;
    while(!bon){
        int x = rand()%(C_WIDTH-2) +1;
        int y = rand()%(C_HEIGHT-2) +1;

        if(chu -> map[y][x] == 0 || chu -> map[y][x] == 2){
            chu -> map[y][x] = 4;
            bon = true;
        }
    }

    chu -> coo_obj_tel = coo;
}



chunk* create_map(int x, int y, dict* d, bool teleporteur, int x_tel, int y_tel);
void init_teleporteur(dict* d, chunk* chu, bool present, int x, int y){
    //Met en place l'objectife du teleporteur si il y en a un
    
    coor coo_obj;
    coo_obj.x = x;
    coo_obj.y = y;

    if(present == false && rand()%100 < 50){
        present = true;
        x = rand()%3 - 15 + chu -> coo.x;
        y = rand()%31 - 15 + chu -> coo.y;

        coo_obj.x = x;
        coo_obj.y = y;

        if(d_mem(d, coo_obj)){
            ajout_teleporteur(d_find(d, coo_obj), chu -> coo);
        }
        else{
            create_map(x,y, d, true, chu -> coo.x, chu -> coo.y);            
        }
    } 
    
    if(present){
        chu -> coo_obj_tel = coo_obj;

        bool bon = false;
        while(!bon){
            int x = rand()%(C_WIDTH-2) +1;
            int y = rand()%(C_HEIGHT-2) +1;

            if(chu -> map[y][x] == 0 || chu -> map[y][x] == 2){
                chu -> map[y][x] = 4;
                bon = true;
            }
        }
    }
}

void init_kaboom(chunk* chu){
    if(rand()%100 < 100){
        bool bon = false;
        while(!bon){
            int x = rand()%(C_WIDTH-2) +1;
            int y = rand()%(C_HEIGHT-2) +1;

            if(chu -> map[y][x] == 0 || chu -> map[y][x] == 2){
                chu -> map[y][x] = 5;
                bon = true;
            }
        }
    }
}

void mur(int** map){
    for(int i = 0; i<C_HEIGHT; i++){
        map[i][0] = 3;
        map[i][C_WIDTH-1] = 3;
    }
    for(int i = 0; i<C_WIDTH; i++){
        map[0][i] = 3;
        map[C_HEIGHT-1][i] = 3;
    }
}



chunk* create_map(int x, int y, dict* d, bool teleporteur, int x_tel, int y_tel){
    //0 pour vide
    //1 pour porte
    //2 pour herbe
    //3 pour mur
    //4 pour teleporteur
    //5 pour destructeur aka Kaboom

    chunk* chu = malloc(sizeof(chunk));
    chu -> map = malloc(sizeof(int*) * C_HEIGHT);
    for(int i = 0; i<C_HEIGHT; i++){
        chu -> map[i] = malloc(sizeof(int) * C_WIDTH);
        for(int j = 0; j < C_WIDTH; j++){
            chu -> map[i][j] = 0;
        }
    }
    
    
    coor coo = {x,y};
    chu -> coo = coo;

    chu -> heigth = C_HEIGHT;
    chu -> width = C_WIDTH;

    chu -> portes_map = NULL;


    //Decorations cartes
    herbe(chu, 35);

    mur(chu -> map);

    init_teleporteur(d, chu, teleporteur, x_tel, y_tel);
    init_kaboom(chu);

    d_add(d, chu);
    return chu;
}


void creation_porte(chunk* chu, dict* d){
    //Création des portes

    //Emplacement des portes :
    // -gauche : 0
    // -haut : 1
    // -droite : 2
    // -bas : 3

    int porte_obj = rand() % 2 + 3 - (rand() % 3 % 2);
    int porte_act = 0;

    chu -> portes_map = malloc(sizeof(chunk*) * 4);

    for(int i = 0; i<4; i++){
        chu -> portes_map[i] = NULL;
    }

    int coo_portes[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    for(int i = 0; i<4; i++){
        coor tmp_coo;

        tmp_coo.x = chu -> coo.x + coo_portes[i][0];
        tmp_coo.y = chu -> coo.y + coo_portes[i][1];
        if(d_mem(d, tmp_coo) == true){          
            chu -> portes_map[i] = d_find(d, tmp_coo);
            chunk* chunk_a = d_find(d, tmp_coo);
            if(chunk_a -> portes_map != NULL){
                chunk_a -> portes_map[(i + 2)%4] = chu;
                actualisation_map(chunk_a);
            }
            
            coo_portes[i][0] = 42;
            
            porte_act++;            
        }
    }

    int i = 0;
    while(porte_act < porte_obj){
        if(coo_portes[i][0] != 42){
            if(rand()%100 < 10){
                chu -> portes_map[i] = create_map(chu -> coo.x + coo_portes[i][0], chu -> coo.y + coo_portes[i][1], d, false, -1, -1);

                coo_portes[i][0] = 42;
                porte_act++;
            }
        }
        i++;
        i= i%4;
    }

    actualisation_map(chu);
}

chunk* delete_chunk(chunk* chu, dict* d){
    d_remove(d, chu -> coo);
    chunk* new_chu = create_map(chu -> coo.x, chu -> coo.y, d, false, -1, -1);
    for(int i = 0; i< 4; i++){
        if(chu -> portes_map != NULL && chu -> portes_map[i] != NULL && chu -> portes_map[i] -> portes_map != NULL){
            chu -> portes_map[i] -> portes_map[(i+2)%4] = new_chu;
        }
    }
    free(chu -> portes_map);
    
    for(int i = 0; i< C_HEIGHT;i++){
        free(chu -> map[i]);
    }
    free(chu -> map);
    free(chu);

    return new_chu;

}

void kaboom_chunk(chunk* chu, dict* d){
    int coo_portes[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    for(int i = 0; i< 4; i++){
        if(chu -> portes_map[i] != NULL){
            chu -> portes_map[i] = delete_chunk(chu -> portes_map[i], d);
        }
    }
}



chunk* move_to(chunk* c, dict* d){
    if (c -> portes_map == NULL){
        creation_porte(c, d);
    }
    printf("%d %d\n", c -> coo.x, c -> coo.y);
    return c;
}


