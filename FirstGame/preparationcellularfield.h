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

    void drawRectangle(sf::RenderWindow* window, std::vector<Ship*>& shipsVector);     // ��������� ����
    void checkCollision(float x, float y, float w, float h, sf::RenderWindow *window); // �������� ������������� �� ������� � �����

    sf::Vector2f getRectPos(); // ������� ������� ���� �������
    bool getCanPutStatus(); // ������� ����� �� ��������� ������� �� ����

    void setIsDrawRectStatus(bool status); // ���������� ����� �� �������� ���� ������� ��� ���


private:
    sf::Vector2f rectPos; // ������� ����
    sf::Vector2f rectSize; // ������ ����

    bool isDrawRect = false; // ����� ���������� ����?
    bool canPutShip = false; // ����� ��������� �������?
};


PreparationCellularField::PreparationCellularField(float x, float y) : CellularField(x, y)
{
}

// �������� ������������� �� ������� � �����
void PreparationCellularField::checkCollision(float x, float y, float w, float h, sf::RenderWindow* window)
{
    sf::FloatRect shipRect(x, y, w, h); // ������� �������
    sf::FloatRect fieldRect(position.x, position.y, CELLS_CNT * CELL_SIZE, CELLS_CNT * CELL_SIZE); // ������� ����
    if (fieldRect.intersects(shipRect)) // ���� ������� ������������� � �����
    {
        // ������ ������� ����
        float startLineX = round((x - position.x) / CELL_SIZE) + 1;
        float endLineX = round((x + w - position.x) / CELL_SIZE) + 1;

        float startLineY = round((y - position.y) / CELL_SIZE) + 1;
        float endLineY = round((y + h - position.y) / CELL_SIZE) + 1;

        rectSize = sf::Vector2f(w, h); // ���������� ������ ����
        rectPos = sf::Vector2f(position.x + (startLineX - 1) * CELL_SIZE, position.y + (startLineY - 1) * CELL_SIZE); // ��������� ������� ����
        isDrawRect = true; // ����� �������� ����
        
    }
    else
        isDrawRect = false; // ������ �������� ����
}

// ��������� ���� �������
void PreparationCellularField::drawRectangle(sf::RenderWindow* window, std::vector<Ship*>& shipsVector)
{
    if (isDrawRect)
    {
        sf::FloatRect rect(rectPos.x, rectPos.y, rectSize.x, rectSize.y); // ������� ����
        sf::FloatRect increasedFieldRect(position.x + rectSize.x - CELL_SIZE,
            position.y + rectSize.y - CELL_SIZE,
            CELLS_CNT * CELL_SIZE - 2 * rectSize.x + 2 * CELL_SIZE, CELLS_CNT * CELL_SIZE - 2 * rectSize.y + 2 * CELL_SIZE); // ����������� ������� ����, ����� ������� ����� ���� ��������� ������ ������
        sf::RectangleShape rectangle(rectSize); // ���� �������
        if (rect.intersects(increasedFieldRect)) // ���� ������� ������ ����
        {
            int i = 0;
            sf::FloatRect increasedShipRect(rectPos.x - CELL_SIZE,
                rectPos.y - CELL_SIZE,
                rectSize.x + 2 * CELL_SIZE,
                rectSize.y + 2 * CELL_SIZE); // ����������� ������� ���� �������, ����� ��� ������ ���� ��������� � �������� ������ � ������ �������
            canPutShip = true; // ����� ��������� �������
            while (canPutShip && i < shipsVector.size())
            {
                sf::Sprite sprite = *(shipsVector[i])->getSprite(); // ��� ������������ �� ���� �������
                if (increasedShipRect.intersects(sprite.getGlobalBounds())) // ���� ������������ � ������ �������
                    canPutShip = false; // ������ ��������� �������
                i++;
            }
        }
        else
        {
            canPutShip = false; canPutShip = false; // ������ ��������� �������
        }

        if (canPutShip) // ���� ����� ��������� �������
            rectangle.setFillColor(sf::Color::Green); // ��������� ���� ������� � ������
        else
            rectangle.setFillColor(sf::Color::Red); // ��������� ���� ������� � �������

        rectangle.setPosition(rectPos); // ��������� ������� ����
        window->draw(rectangle); // ��������� ����
    }
}

// ���������� ����� �� �������� ���� ������� ��� ���
void PreparationCellularField::setIsDrawRectStatus(bool status)
{
    isDrawRect = status;
}

// ������� ����� �� ��������� ������� �� ����
bool PreparationCellularField::getCanPutStatus()
{
    return canPutShip;
}

// ������� ������� ���� �������
sf::Vector2f PreparationCellularField::getRectPos()
{
    return rectPos;
}