#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <utility>

typedef std::vector<std::pair<int,int>> VectorCamino;
typedef std::pair<int,int> coord;

struct Casilla
{
    char tipo{'e'};
    int heuristica{0};
    bool visitado{false};
};

struct Mapa
{
    int filas;
    int columnas;

    std::vector<std::vector<Casilla>> map;

    Mapa(int f = 0, int c = 0)
    {
        filas = f;
        columnas = c;

        map.resize(filas, std::vector<Casilla>(columnas));
    }
};

void imprimirMapaTipo(const Mapa& mapaImprimir);

void imprimirMapaHeu(const Mapa& mapaImprimir);

void limpiarPantalla();

void reiniciarMapa(Mapa& mapaGlobal);

void aplicarHeu(Mapa& mapaGlobal,coord& startPoint,coord& goalPoint);

void definirHeuUsuario(Mapa& mapaGlobal);

bool definirMapa(int& numObs,std::pair<int,int>& startPoint,std::pair<int,int>& goalPoint,Mapa& mapaGlobal);

bool caminoDisponible(std::vector<bool>& disponibles);

std::vector<bool> evaluarVecinos(std::pair<int,int>& actualPoint,std::pair<int,int>& up,std::pair<int,int>& down,std::pair<int,int>& left,std::pair<int,int>& right,
    Mapa& mapaGlobal);

void Astar(Mapa& mapaGlobal,std::pair<int,int>& actualPoint,std::pair<int,int>& goalPoint);

#endif