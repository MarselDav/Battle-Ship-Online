#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "cellularfield.h"
#include "ship.h"


class PreparationCellularField : public CellularField
{
public:
    PreparationCellularField(float x, float y);

    void drawRectangle(sf::RenderWindow* window, std::vector<Ship*>& shipsVector);     // отрисовка тени
    void checkCollision(float x, float y, float w, float h, sf::RenderWindow *window); // проверка соприкасается ли корабль с полем

    sf::Vector2f getRectPos(); // поучить позицию тени корабля
    bool getCanPutStatus(); // вернуть можно ли поставить корабль на стол

    void setIsDrawRectStatus(bool status); // установить можно ли рисовать тень корабля или нет


private:
    sf::Vector2f rectPos; // позиция тени
    sf::Vector2f rectSize; // размер тени

    bool isDrawRect = false; // можно нарисовать тень?
    bool canPutShip = false; // можно поставить корабль?
};


PreparationCellularField::PreparationCellularField(float x, float y) : CellularField(x, y)
{
}

// проверка соприкасается ли корабль с полем
void PreparationCellularField::checkCollision(float x, float y, float w, float h, sf::RenderWindow* window)
{
    sf::FloatRect shipRect(x, y, w, h); // квадрат корабля
    sf::FloatRect fieldRect(position.x, position.y, CELLS_CNT * CELL_SIZE, CELLS_CNT * CELL_SIZE); // квадрат поля
    if (fieldRect.intersects(shipRect)) // если корабль соприкасается с полем
    {
        // расчёт позиции тени
        float startLineX = round((x - position.x) / CELL_SIZE) + 1;
        float endLineX = round((x + w - position.x) / CELL_SIZE) + 1;

        float startLineY = round((y - position.y) / CELL_SIZE) + 1;
        float endLineY = round((y + h - position.y) / CELL_SIZE) + 1;

        rectSize = sf::Vector2f(w, h); // установить размер тени
        rectPos = sf::Vector2f(position.x + (startLineX - 1) * CELL_SIZE, position.y + (startLineY - 1) * CELL_SIZE); // установка позиции тени
        isDrawRect = true; // можно рисовать тень
        
    }
    else
        isDrawRect = false; // нельзя рисовать тень
}

// отрисовка тени корабля
void PreparationCellularField::drawRectangle(sf::RenderWindow* window, std::vector<Ship*>& shipsVector)
{
    if (isDrawRect)
    {
        sf::FloatRect rect(rectPos.x, rectPos.y, rectSize.x, rectSize.y); // квадрат тени
        sf::FloatRect increasedFieldRect(position.x + rectSize.x - CELL_SIZE,
            position.y + rectSize.y - CELL_SIZE,
            CELLS_CNT * CELL_SIZE - 2 * rectSize.x + 2 * CELL_SIZE, CELLS_CNT * CELL_SIZE - 2 * rectSize.y + 2 * CELL_SIZE); // увеличенный квадрат поля, чтобы корабль можно было поставить строго внутри
        sf::RectangleShape rectangle(rectSize); // тенб корабля
        if (rect.intersects(increasedFieldRect)) // если корабль внутри поля
        {
            int i = 0;
            sf::FloatRect increasedShipRect(rectPos.x - CELL_SIZE,
                rectPos.y - CELL_SIZE,
                rectSize.x + 2 * CELL_SIZE,
                rectSize.y + 2 * CELL_SIZE); // увеличенный квадрат тени корабля, чтобы его нельзя было поставить в соседнюю клетку с другим кораблём
            canPutShip = true; // можно поставить корабль
            while (canPutShip && i < shipsVector.size())
            {
                sf::Sprite sprite = *(shipsVector[i])->getSprite(); // уже поставленный на стол корабль
                if (increasedShipRect.intersects(sprite.getGlobalBounds())) // если пересекается с другим кораблём
                    canPutShip = false; // нельзя поставить корабль
                i++;
            }
        }
        else
        {
            canPutShip = false; canPutShip = false; // нельзя поставить корабль
        }

        if (canPutShip) // если можно поставить корабль
            rectangle.setFillColor(sf::Color::Green); // закрасить тень корабля в зелёный
        else
            rectangle.setFillColor(sf::Color::Red); // закрасить тень корабля в красный

        rectangle.setPosition(rectPos); // установка позиции тени
        window->draw(rectangle); // отрисовка тени
    }
}

// установить можно ли рисовать тень корабля или нет
void PreparationCellularField::setIsDrawRectStatus(bool status)
{
    isDrawRect = status;
}

// вернуть можно ли поставить корабль на стол
bool PreparationCellularField::getCanPutStatus()
{
    return canPutShip;
}

// поучить позицию тени корабля
sf::Vector2f PreparationCellularField::getRectPos()
{
    return rectPos;
}