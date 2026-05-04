#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include "struct.h"

//Definiciones previas de funciones (pasar a .h y .cpp despues)
void imprimirMapaTipo(const Mapa& mapaImprimir, const int& ancho, const int& largo);

void limpiarPantalla();

void reiniciarMapa(Mapa& mapaGlobal,const int& ancho, const int& largo);

int calcularHeu(std::pair<int,int> point1,std::pair<int,int> point2);

void aplicarHeu(Mapa& mapaGlobal,std::pair<int,int>& startPoint,std::pair<int,int>& goalPoint,
    const int& ancho, const int& largo);

bool definirMapa(int& ancho,int& largo,int& numObs,std::pair<int,int>& startPoint,
    std::pair<int,int>& goalPoint,Mapa& mapaGlobal);

void imprimirMapaHeu(const Mapa& mapaImprimir, const int& ancho, const int& largo);

void evaluarVecinos(std::pair<int,int>& actualPoint,std::pair<int,int>& up,std::pair<int,int>& down,std::pair<int,int>& left,std::pair<int,int>& right,
    int* ptrA,int* ptrL,Mapa& mapaGlobal);

void Astar(Mapa& mapaGlobal,std::pair<int,int>& actualPoint,std::pair<int,int>& goalPoint,
    int ancho,int largo);

void imprimirPose(const Mapa& mapaImprimir, const int& ancho, const int& largo,
     const std::pair<int,int>& actualPoint);

//Funciones 

bool definirMapa(int& ancho,int& largo,int& numObs,std::pair<int,int>& startPoint,std::pair<int,int>& goalPoint,Mapa& mapaGlobal)
{
    int x{};
    int y{};
    int i{0};
    std::vector<int> obstaculosX{};
    std::vector<int> obstaculosY{};

    std::cout<<"Ingrese las dimensiones del mapa: \n";
    std::cout<<"Ancho: \n";
    std::cin>>ancho;

    std::cout<<"Largo: \n";
    std::cin>>largo;

    std::cout<<"Numero de Obstaculos: \n";
    std::cin>>numObs;

    mapaGlobal.map[ancho][largo]={};

    //Si se quieren poner obstaculos se cuida que no se salgan del mapa
    if(numObs>=1)
    {
        //Limpiamos la memoria de los vectores
        obstaculosX.clear();
        obstaculosY.clear();

        for(int i{0};i<numObs;i++)
        {
            //Checar si x esta dentro del rango
            printf("Obstaculo numero %d, ingrese coordenada x: ",i);
            std::cin>>x;
            if(x>=ancho)
            {
                reiniciarMapa(mapaGlobal,ancho,largo);
                std::cout<<"Valor invalido, intenta de nuevo \n";
                return false;
            }
            obstaculosX.push_back(x);

            //Checar si y esta dentro del rango
            printf("Obstaculo numero %d, ingrese coordenada y: ",i);
            std::cin>>y;
            if(y>=largo)
            {
                reiniciarMapa(mapaGlobal,ancho,largo);
                std::cout<<"Valor invalido, intenta de nuevo \n";
                return false;
            }
            obstaculosY.push_back(y);
        }
        
        for(int i{0};i<obstaculosX.size();i++)
        {
            mapaGlobal.map[obstaculosX.at(i)][obstaculosY.at(i)] = {'o',0};
        }
    }

    imprimirMapaTipo(mapaGlobal,ancho,largo);

    //Una vez que definimos los obstaculos, vamos a poner el start y goal
    //Checar si start esta dentro del mapa
    std::cout<<"Ingrese el punto de inicio con coordenadas: \n";
    std::cout<<"Coordenada x para start: \n";
    std::cin>>x;
    if(x>=ancho)
    {
        reiniciarMapa(mapaGlobal,ancho,largo);
        std::cout<<"Valor invalido, intenta de nuevo \n";
        return false;
    }

    std::cout<<"Coordenada y para start: \n";
    std::cin>>y;
    if(y>=largo)
    {
        reiniciarMapa(mapaGlobal,ancho,largo);
        std::cout<<"Valor invalido, intenta de nuevo \n";
        return false;
    }
    //Si esta bien guardamos las coordenadas
    startPoint.first=x;
    startPoint.second=y;

    //Ahora los agregamos al mapa, checando que  
    //la coordenada no este sobre un obstaculo 
    if(mapaGlobal.map[startPoint.first][startPoint.second].tipo!='o')
    {
        mapaGlobal.map[startPoint.first][startPoint.second]={'s'};

    }
    else
    {
        reiniciarMapa(mapaGlobal,ancho,largo);
        std::cout<<"El punto de inicio no puede ir sobre un obstaculo \n";
        return false;
    }

    //Checar si goal esta dentro del mapa
    std::cout<<"Ingrese el punto de la meta con coordenadas: \n";
    std::cout<<"Coordenada x para goal: \n";
    std::cin>>x;
    if(x>=ancho)
    {
        reiniciarMapa(mapaGlobal,ancho,largo);
        std::cout<<"Valor invalido, intenta de nuevo \n";
        return false;
    }
    std::cout<<"Coordenada y para goal: \n";
    std::cin>>y;
    if(y>=largo)
    {
        reiniciarMapa(mapaGlobal,ancho,largo);
        std::cout<<"Valor invalido, intenta de nuevo \n";
        return false;
    }
    //Si esta bien guardamos las coordenadas

    goalPoint.first=x;
    goalPoint.second=y;
    
    if(mapaGlobal.map[goalPoint.first][goalPoint.second].tipo!='o')
    {
        mapaGlobal.map[goalPoint.first][goalPoint.second]={'g'};
    }
    else
    {
        reiniciarMapa(mapaGlobal,ancho,largo);
        std::cout<<"La meta no puede ir sobre un obstaculo \n";
        return false;
    }

    return true;
}

int calcularHeu(std::pair<int,int> point1,std::pair<int,int> point2)
{
    int heursitica{0};
    heursitica= (abs(point1.first-point2.first))+(abs(point1.second-point2.second));
    return heursitica;
}

void imprimirMapaTipo(const Mapa& mapaImprimir, const int& ancho, const int& largo)
{
    Casilla valor{};
    std::cout<<'\n';
    std::cout<<"Mapa: e->espacio libre, o->obstaculo, s->start, g->goal\n";

    for(int i{0};i<ancho;i++)
    {
        for(int j{0};j<largo;j++)
        {
            valor.tipo= mapaImprimir.map[i][j].tipo;

            std::cout<<valor.tipo<<" ";
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';

}


void limpiarPantalla()
{
    system("clear");
}

void reiniciarMapa(Mapa& mapaGlobal,const int& ancho, const int& largo)
{

    for(int i{0};i<ancho;i++)
    {
        for(int j{0};j<largo;j++)
        {
            mapaGlobal.map[i][j].tipo ='e';
        }
    }
    //limpiarPantalla();
}

void aplicarHeu(Mapa& mapaGlobal,std::pair<int,int>& startPoint,std::pair<int,int>& goalPoint,
    const int& ancho, const int& largo)
{
    int heuristica{0};
    std::pair<int,int> newPoint={};

    for(int i{0};i<ancho;i++)
    {
        for(int j{0};j<largo;j++)
        {
            newPoint.first=i;
            newPoint.second=j;

            heuristica=calcularHeu(goalPoint,newPoint);
            mapaGlobal.map[i][j].heuristica=heuristica;
        }
    }


}

void imprimirMapaHeu(const Mapa& mapaImprimir, const int& ancho, const int& largo)
{
    Casilla valor{};
    std::cout<<'\n';
    std::cout<<"Heuristica del Mapa\n";

    for(int i{0};i<ancho;i++)
    {
        for(int j{0};j<largo;j++)
        {
            valor.heuristica= mapaImprimir.map[i][j].heuristica;

            std::cout<<valor.heuristica<<" ";
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}


void Astar(Mapa& mapaGlobal,std::pair<int,int>& actualPoint,std::pair<int,int>& goalPoint,
    int ancho,int largo)
{
    std::pair<int,int> up={};
    std::pair<int,int> down={};
    std::pair<int,int> left={};
    std::pair<int,int> right={};

    std::vector<std::pair<int,int>> path ={};

    int* ptrAncho = &ancho;
    int* ptrLargo = &largo;
    std::pair<int,int> opciones[4]={};

    //Matriz para marcar las celdas visitadas, valores iniciales false
    bool visitados[ancho][largo]={};
    //Posicion inicial visitada
    visitados[actualPoint.first][actualPoint.second]=true;

    int dist{0}; // distancia que toma llegar desde el start hasta la casilla que sige
    int temporal{0};
    int heuMinima{0};
    int indice{0};
    bool sinCamino=false;

    while((mapaGlobal.map[actualPoint.first][actualPoint.second].tipo!='g'))
    {
        //Se evaluan los vecinos del punto actual, para saber a donde puede ir
        evaluarVecinos(actualPoint,up,down,left,right,ptrAncho,ptrLargo,mapaGlobal);

        /*/Ahora vamos a calcular el costo de ir a un vecino tomando en cuenta la distancia y la heuristica
        quedandonos con el de menor costo
        */
        opciones[0] = up;
        opciones[1] = down;
        opciones[2] = left;
        opciones[3] = right;

        
        dist++;// distancia que toma llegar desde el start hasta la casilla que sigue
        temporal=0;
        heuMinima=99990;
        indice=0;

        //Si el punto actual tiene al menos un vecino disponible entonces seguimos con el codigo

        // F = G + H
        for(int i{0};i<4;i++)
        {

            /*Cuando se evaluaron los vecinos se asigno un valor de -1 cuando una casilla no era accesible
            entonces si eso es negativo saltara a la siguiente*/
            if(opciones[i].first<0) //Si la casilla no se puede acceder, pero todavia no sabemos porque
            {
                continue;
            }
            else if(mapaGlobal.map[opciones[i].first][opciones[i].second].tipo=='o') 
            {
                visitados[opciones[i].first][opciones[i].second] = true;
                continue;
            }
            
            //Aqui ya se calcula el valor que tomaria llegar a una casilla
            temporal = dist+mapaGlobal.map[opciones[i].first][opciones[i].second].heuristica;
            /*Se actualiza cuando se encuentra un valor mas pequeno a donde ir y se guarda el indice, siempre y cuando no haya
            sido visitado antes*/
            if((visitados[opciones[i].first][opciones[i].second]==false) && (temporal<heuMinima))
            {
                heuMinima=temporal;
                indice = i;
            }
        }

        /*Si todo va bien ya tendriamos las coordenadas con el menor costo, revisamos que no haya sido visitada
        Actualizamos la posicion actual y marcamos las coordenadas como visitada*/

        actualPoint = opciones[indice];
        visitados[actualPoint.first][actualPoint.second] = true;
        //Ahora vamos agregando el nodo actual a path
        path.push_back(actualPoint);
    }

    std::cout<<"Se encontro un caminoooo!!!!!!!!!!!!!!!!!\n";

    bool valor{0};

    for(int i{0};i<ancho;i++)
    {
        for(int j{0};j<largo;j++)
        {
            valor = visitados[i][j];

            if(valor==1){
                std::cout<<'v'<<" ";
            }else{
                std::cout<<'n'<<" ";
            }
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

//En esta funcion falta implementar que los obstaculos tambien son innacesibles
void evaluarVecinos(std::pair<int,int>& actualPoint,std::pair<int,int>& up,std::pair<int,int>& down,std::pair<int,int>& left,std::pair<int,int>& right,
    int* ptrA,int* ptrL,Mapa& mapaGlobal)
{
     /*Aqui tenemos que evaluar si al momento de buscar los vecinos estos no se salgan del mapa 
    o que no sean obstaculos
    
    lista de casos posibles:
    1)la posicion actual se encuentra en el limite izquierdo del mapa -> su la coordenada y<=0
    2)la posicion actual se encuentra en el limite derecho del mapa -> su la coordenada y>=largo
    3)la posicion actual se encuentra en el limite superior del mapa -> su la coordenada x<=0
    4)la posicion actual se encuentra en el limite inferior del mapa -> su la coordenada x>=ancho
    5)la(s) casilla(s) de al lado == obstaculo
    */

    if(actualPoint.first>0) //si puede ir para arriba
    {
        //Verificamos que la coordenada que vamos a poner no es un obstaculo
        if(mapaGlobal.map[actualPoint.first-1][actualPoint.second].tipo!='o')
        {
            up.first = actualPoint.first-1;
            up.second = actualPoint.second;
            mapaGlobal.map[actualPoint.first][actualPoint.second].vecinosDisponibles++;
        }else //Si es un obstaculo le asignamos estos valores que nos ayudara mas tarde
        {
            up.first = -(actualPoint.first-1);
            up.second = -(actualPoint.second);
        }
    }else//si esta, entonces no puede ir para arriba
    {
        up.first = -99999;
        up.second = -99999;
    }
    
    if(actualPoint.first<((*ptrA)-1)) // si puede ir para abajo
    {
        //Verificamos que la coordenada que vamos a poner no es un obstaculo
        if((mapaGlobal.map[actualPoint.first+1][actualPoint.second].tipo) !='o')
        {
            down.first = actualPoint.first+1;
            down.second = actualPoint.second;
            //Vamos contando los vecinos disponibles de cada celda
            mapaGlobal.map[actualPoint.first][actualPoint.second].vecinosDisponibles++;
        }else //Si es un obstaculo le asignamos estos valores que nos ayudara mas tarde
        {
            down.first = -(actualPoint.first+1);
            down.second = -(actualPoint.second);
        }
    }else //si esta, entonces no puede ir para abajo
    {
        down.first = -99999;
        down.second = -99999;
    }
    

    if(actualPoint.second>0)
    {
        if((mapaGlobal.map[actualPoint.first][actualPoint.second-1].tipo) != 'o')
        {
            left.first = actualPoint.first;
            left.second = actualPoint.second-1;
            //Vamos contando los vecinos disponibles de cada celda
            mapaGlobal.map[actualPoint.first][actualPoint.second].vecinosDisponibles++;
        }else
        {
            left.first = -(actualPoint.first);
            left.second = -(actualPoint.second-1);
        }
        
    }else
    {
        left.first = -99999;
        left.second = -99999;
    }


    if(actualPoint.second<((*ptrL)-1))
    {
        if((mapaGlobal.map[actualPoint.first][actualPoint.second+1].tipo) != 'o')
        {
            right.first = actualPoint.first;
            right.second = actualPoint.second+1;
            //Vamos contando los vecinos disponibles de cada celda
            mapaGlobal.map[actualPoint.first][actualPoint.second].vecinosDisponibles++;
        }else
        {
            right.first = -(actualPoint.first);
            right.second = -(actualPoint.second+1);
        }
        
    }else
    {
        right.first = -99999;
        right.second = -99999;
    }
}


int main(){

    int ancho{0};
    int largo{0};
    int numObs{0};
    bool flag{true};
    std::pair<int,int> startPoint={};
    std::pair<int,int> goalPoint={};
    Mapa mapaGlobal = {};
    
    
    do
    {
        flag = definirMapa(ancho,largo,numObs,startPoint,goalPoint,mapaGlobal);
    }while (flag==0);

    /*Para este punto ya debemos tener el mapa correcto
    Incluyendo obstaculos, punto de inicio y meta
    Tomando en cuenta que el mapa se paso por refrencia entonces ya esta todo guardado ahi*/
    
    imprimirMapaTipo(mapaGlobal,ancho,largo);

    /*Siguiente paso seria incluir la heuristica a cada casilla del mapa*/

    aplicarHeu(mapaGlobal,startPoint,goalPoint,ancho,largo);
    imprimirMapaHeu(mapaGlobal,ancho,largo);

    /*LISTOOOO, ahora que sigue? APLICAR A*!!!*/

    Astar(mapaGlobal,startPoint,goalPoint,ancho,largo);

    return 0;
}
