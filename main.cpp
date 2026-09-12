#include <iostream>
#include <SFML/Graphics.hpp>
#include "Render.h"
#include "Astar.h"

/*Comando para compilar 
g++ main.cpp Astar.cpp Render.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system*/

int main(){

    int numObs{0};
    bool flag{true};
    int choose{0};
    std::pair<int,int> startPoint={};
    std::pair<int,int> goalPoint={};
    Mapa mapaGlobal;
    
    
    do
    {
        flag = definirMapa(numObs,startPoint,goalPoint,mapaGlobal);
    }while (flag==0);
    
    imprimirMapaTipo(mapaGlobal);

    /*Siguiente paso seria incluir la heuristica a cada casilla del mapa*/

    std::cout<<"Quieres aplicar heuristca manualmente o automaticamente? \n";
    std::cout<<"1->manualmente     2->automaticamente \n";
    std::cin>>choose;
    if(choose==1)
    {
        definirHeuUsuario(mapaGlobal);

    }else if (choose==2)
    {
        aplicarHeu(mapaGlobal,startPoint,goalPoint);
    }else{
        std::cout<<"Opcion no valida \n";
        return 0;
    }
    
    imprimirMapaHeu(mapaGlobal);


    Astar(mapaGlobal,startPoint,goalPoint);

    sf::RenderWindow window(sf::VideoMode(800, 600), "A* Visualizer");

    while(window.isOpen())
    {
        sf::Event event;

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }        
        }

        window.clear();

        dibujarMapa(window,mapaGlobal);

        window.display();
        
    }


    return 0;
}
