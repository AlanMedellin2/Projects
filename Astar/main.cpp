#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include "struct.h"

typedef std::vector<std::pair<int,int>> VectorCamino;
typedef std::pair<int,int> coord;


//Definiciones previas de funciones (pasar a .h y .cpp despues)
void imprimirMapaTipo(const Mapa& mapaImprimir, const int& ancho, const int& largo);

void limpiarPantalla();

void reiniciarMapa(Mapa& mapaGlobal,const int& ancho, const int& largo);

void aplicarHeu(Mapa& mapaGlobal,coord& startPoint,coord& goalPoint,
    const int& ancho, const int& largo);

bool definirMapa(int& ancho,int& largo,int& numObs,std::pair<int,int>& startPoint,
    std::pair<int,int>& goalPoint,Mapa& mapaGlobal);

void imprimirMapaHeu(const Mapa& mapaImprimir, const int& ancho, const int& largo);

bool caminoDisponible(std::vector<bool>& disponibles);

std::vector<bool> evaluarVecinos(std::pair<int,int>& actualPoint,std::pair<int,int>& up,std::pair<int,int>& down,std::pair<int,int>& left,std::pair<int,int>& right,
    int* ptrA,int* ptrL,Mapa& mapaGlobal);

void Astar(Mapa& mapaGlobal,std::pair<int,int>& actualPoint,std::pair<int,int>& goalPoint,
    int ancho,int largo);


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


void imprimirMapaTipo(const Mapa& mapaImprimir, const int& ancho, const int& largo)
{
    Casilla valor{};
    std::cout<<'\n';
    std::cout<<"Mapa: e->espacio libre, o->obstaculo, s->start, g->goal, p->path\n";

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

void aplicarHeu(Mapa& mapaGlobal,coord& startPoint,coord& goalPoint,
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

            heuristica= (abs(goalPoint.first-newPoint.first))+(abs(goalPoint.second-newPoint.second));
            mapaGlobal.map[i][j].heuristica=heuristica;
        }
    }
}

bool caminoDisponible(std::vector<bool>& disponibles)
{
    for(int i{0};i<4;i++)
    {
        if(disponibles[i]==true)
        {
            return true;
        }
    }
    return false;
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

    VectorCamino path ={};

    int* ptrAncho = &ancho;
    int* ptrLargo = &largo;
    std::pair<int,int> opciones[4]={};

    //Posicion inicial visitada
    mapaGlobal.map[actualPoint.first][actualPoint.second].visitado=true;
    path.push_back(actualPoint);

    int dist{0}; // distancia que toma llegar desde el start hasta la casilla que sige
    int temporal{0};
    int heuMinima{0};
    int indice{0};
    std::vector<bool> disponibles(4);
    int contadorDisponibles{0};
    bool flagContinuar{false};
    bool terminar {false};
    int sizeVec {0};


    while((mapaGlobal.map[actualPoint.first][actualPoint.second].tipo!='g'))
    {

        dist++;// distancia que toma llegar desde el start hasta la casilla que sigue
        temporal=0;
        heuMinima=99999;
        indice=0;
        contadorDisponibles=0;

        //Se evaluan los vecinos del punto actual, para saber a donde puede ir
        disponibles = evaluarVecinos(actualPoint,up,down,left,right,ptrAncho,ptrLargo,mapaGlobal);

        /*/Ahora vamos a calcular el costo de ir a un vecino tomando en cuenta la distancia y la heuristica
        quedandonos con el de menor costo
        */
        opciones[0] = up;
        opciones[1] = left;
        opciones[2] = down;
        opciones[3] = right;

        //Si el punto actual tiene al menos un vecino disponible entonces seguimos con el codigo
        flagContinuar = caminoDisponible(disponibles);

        if (flagContinuar==false) //Si no hay camino, vamos a retroceder en el vector de path
        {
            if(path.empty()) //Si el vector esta vacio, ya no hay caminos disponibles
            {
                std::cout<<"Ya no hay caminos disponibles \n";
                return;
            }else
            {
                sizeVec = path.size();
                int indice{sizeVec-1}; //Tamano del vector de camino

                //Vamos a recorrer el camino hacia atras, hasta encontrar uno con vecinos disponibles por explorar
                for(int i{indice};i>=0;i--)
                {
                    disponibles = evaluarVecinos(path[i],up,down,left,right,ptrAncho,ptrLargo,mapaGlobal);
                    flagContinuar = caminoDisponible(disponibles);
                    if(flagContinuar==true)
                    {
                        actualPoint = path[i];
                        goto contunuarAqui;
                    }
                    else{
                        path.pop_back();
                    }
                }
                //Si no se encontro un punto con vecinos disponibles
                return;
            }
        }


        contunuarAqui:
        // F = G + H
        for(int i{0};i<4;i++)
        {

            /* */
            if(disponibles[i]==true) 
            {
                //Aqui ya se calcula el valor que tomaria llegar a una casilla
                temporal = dist+mapaGlobal.map[opciones[i].first][opciones[i].second].heuristica;
                /*Se actualiza cuando se encuentra un valor mas pequeno a donde ir y se guarda el indice, siempre y cuando no haya
                sido visitado antes*/
                if(temporal<heuMinima)
                {
                    heuMinima=temporal;
                    indice = i;

                }
            }
        }

        /*Si todo va bien ya tendriamos las coordenadas con el menor costo
        Actualizamos la posicion actual y marcamos las coordenadas como visitada*/

        actualPoint = opciones[indice];
        mapaGlobal.map[actualPoint.first][actualPoint.second].visitado = true;

        //Ahora vamos agregando el nodo actual a path
        path.push_back(actualPoint);

    }

    std::cout<<"Se encontro un caminoooo!!!!!!!!!!!!!!!!!\n";

    sizeVec = path.size();

    for(int i{0};i<sizeVec;i++)
    {
        mapaGlobal.map[path[i].first][path[i].second].tipo = 'p';
    }

    imprimirMapaTipo(mapaGlobal,ancho,largo);

    std::cout<<'\n';

    return;

}


std::vector<bool> evaluarVecinos(std::pair<int,int>& actualPoint,std::pair<int,int>& up,std::pair<int,int>& down,std::pair<int,int>& left,std::pair<int,int>& right,
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

    std::vector<bool> disponibles(4); //Inicializado por defecto en false

    if(actualPoint.first>0) //si puede ir para arriba
    {
        
        up.first = actualPoint.first-1;
        up.second = actualPoint.second;
        if(mapaGlobal.map[up.first][up.second].visitado==false && mapaGlobal.map[up.first][up.second].tipo!='o')
        {
            disponibles[0] = true;
        }
    }

    if(actualPoint.second>0)  //si puede ir para la izquierda
    {
        
        left.first = actualPoint.first;
        left.second = actualPoint.second-1;
        if(mapaGlobal.map[left.first][left.second].visitado==false && mapaGlobal.map[left.first][left.second].tipo!='o')
        {
            disponibles[1] = true;
        }
        
    }


    if(actualPoint.first<((*ptrA)-1)) // si puede ir para abajo
    {
        
        down.first = actualPoint.first+1;
        down.second = actualPoint.second;
        if(mapaGlobal.map[down.first][down.second].visitado==false && mapaGlobal.map[down.first][down.second].tipo!='o')
        {
            disponibles[2] = true;
        }
        
    }


    if(actualPoint.second<((*ptrL)-1))  //si puede ir para la derecha
    {
       
        right.first = actualPoint.first;
        right.second = actualPoint.second+1;

        if(mapaGlobal.map[right.first][right.second].visitado==false && mapaGlobal.map[right.first][right.second].tipo!='o')
        {
            disponibles[3] = true;
        }
    }

    return disponibles;
}

void evaluarCamino(std::pair<int,int> opciones[4])
{
    for(int i{0};i<4;i++)
    {
        if(opciones[i].first==-1)
        {

        }
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
