#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <list>
#include <array>

#include "constants.h"
#include "ship.h"


class CellularField
{
public:
    CellularField(float x, float y);

    void drawField(sf::RenderWindow* window);

    sf::Vector2f getPosition();
    sf::Vector2i getCellNumber(sf::Vector2i mousePos);
    sf::FloatRect getRect();

private:
    std::vector<std::vector<int>> shipsConfirmed;

protected:
    sf::Vector2f position;

    sf::RectangleShape frame;
    sf::Text simbol;

};


CellularField::CellularField(float x, float y)
{
    position = sf::Vector2f(x, y);

    frame.setSize(sf::Vector2f(CELL_SIZE * CELLS_CNT, CELL_SIZE * CELLS_CNT));
    frame.setOutlineThickness(FRAME_THICKNESS);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::Black);
    frame.setPosition(position);

    simbol.setFont(font);
    simbol.setFillColor(sf::Color(96, 115, 162));
}


void CellularField::drawField(sf::RenderWindow* window)
{
    for (int i = 0; i < CELLS_CNT; i++)
    {
        sf::VertexArray verticalline(sf::Lines, 2); // создаём линию
        verticalline[0].position = sf::Vector2f(position.x + i * CELL_SIZE, position.y);
        verticalline[0].color = LINE_COLOR;
        verticalline[1].position = sf::Vector2f(position.x + i * CELL_SIZE, position.y + CELLS_CNT * CELL_SIZE);
        verticalline[1].color = LINE_COLOR;
        window->draw(verticalline);

        sf::VertexArray gorizontalline(sf::Lines, 2 ); // создаём линию
        gorizontalline[0].position = sf::Vector2f(position.x, position.y + i * CELL_SIZE);
        gorizontalline[0].color = LINE_COLOR;
        gorizontalline[1].position = sf::Vector2f(position.x + CELLS_CNT * CELL_SIZE, position.y + i * CELL_SIZE);
        gorizontalline[1].color = LINE_COLOR;
        window->draw(gorizontalline);

        simbol.setString(lettersList[i]);
        simbol.setPosition(position.x - CELL_SIZE, position.y + i * CELL_SIZE);
        window->draw(simbol);

        simbol.setString(std::to_string(i + 1));
        simbol.setPosition(position.x + i * CELL_SIZE + (CELL_SIZE - simbol.getGlobalBounds().width) / 2, position.y + CELL_SIZE * CELLS_CNT);
        window->draw(simbol);

    }

    window->draw(frame);
}

sf::Vector2i CellularField::getCellNumber(sf::Vector2i mousePos)
{
    return sf::Vector2i((mousePos.x - position.x) / CELL_SIZE, (mousePos.y - position.y) / CELL_SIZE);
}

sf::Vector2f CellularField::getPosition()
{
    return position;
}

sf::FloatRect CellularField::getRect()
{
    return sf::FloatRect(position.x, position.y, CELLS_CNT * CELL_SIZE, CELLS_CNT * CELL_SIZE);
}
