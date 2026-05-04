#ifndef STRUCT
#define STRUCT

struct Casilla
{
    /*e=espacio libre
      o=obstaculo
      s=start
      g=goal
    */
    char tipo{'e'}; 
    int heuristica{0};
    int vecinosDisponibles{0};

};

struct Mapa
{
    Casilla map[5][5]={};
};

#endif
