#include <SFML/Graphics.hpp>
#include "Render.h"

void dibujarMapa(sf::RenderWindow& window, Mapa& mapa)
{
    sf::RectangleShape cell(sf::Vector2f(40.f, 40.f));
    for(int i{0}; i<mapa.filas; i++)
        {
            for(int j{0}; j<mapa.columnas; j++)
            {
                sf::RectangleShape cell(sf::Vector2f(40.f,40.f));

                cell.setPosition(j*40, i*40);

                cell.setOutlineThickness(1.f);

                cell.setOutlineColor(sf::Color::Black);

                char tipo = mapa.map[i][j].tipo;

                if(tipo == 'e')
                    cell.setFillColor(sf::Color::White);

                else if(tipo == 'o')
                    cell.setFillColor(sf::Color::Magenta);

                else if(tipo == 's')
                    cell.setFillColor(sf::Color::Green);

                else if(tipo == 'g')
                    cell.setFillColor(sf::Color::Red);

                else if(tipo == 'p')
                    cell.setFillColor(sf::Color::Blue);

                window.draw(cell);
            }
        }
}

