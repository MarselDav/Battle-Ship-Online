#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "constants.h"
#include "object.h"


class Ship : public Object
{
public:
	Ship(float x, float y, float width, float height, std::string imagefile, bool isMovable);

	void update(sf::Vector2f mousePos); // обновление позиции корабля
	void rotate(); // вращение корабля

	void setStartMousePos(sf::Vector2f mousePos); // установить стартовую позицию мыши

	sf::Vector2i getPositionInCells(sf::Vector2f cellularFieldPos); // вернуть позицию корабля в клетках
	std::string getImageFile(); // вернуть название файла спрайта корабля

private:
	sf::Vector2f m_startMousePos; // стартовая позиция мыши
	std::string m_imagefile; // название файла спрайта корабля
	bool m_isMovable; // флаг можно ли двигать корабль
};

Ship::Ship(float x, float y, float width, float height, std::string imagefile, bool isMovable) : Object(x, y, width, height, imagefile)
{
	m_isMovable = isMovable; // флаг можно ли двигать корабль
	m_imagefile = imagefile; // название файла спрайта корабля
}

void Ship::rotate() // вращение корабля
{
	if (sprite.getRotation() == ROTATE_ANGLE) // если угол спрайта равен 90 градусов
	{
		sprite.setRotation(0); // вернуть в исходное положение
		sprite.setOrigin(0, 0); // установить точку отчёта параметров корабля по умолчанию
	}
	else
	{
		sprite.setRotation(ROTATE_ANGLE); // повернуть на угол 90 градусов
		sprite.setOrigin(0, sprite.getGlobalBounds().width); // установить новую точку отчёта параметров корабля, подвинутую на ширину корабля
	}
}

void Ship::update(sf::Vector2f mousePos) // обновление позиции корабля
{
	if (m_isMovable) // если корабль может двигаться
	{
		sprite.setPosition(sprite.getPosition().x + mousePos.x - m_startMousePos.x, // изменить позицию корабля на позицию мыши
			sprite.getPosition().y + mousePos.y - m_startMousePos.y);
		m_startMousePos = mousePos; // изменить стартовую позицию мыши на текущую
	}
}

sf::Vector2i Ship::getPositionInCells(sf::Vector2f cellularFieldPos) // вернуть позицию корабля в клетках
{
	return sf::Vector2i((sprite.getPosition().x - cellularFieldPos.x) / CELL_SIZE ,
		(sprite.getPosition().y - cellularFieldPos.y) / CELL_SIZE); // вернуть позицию корабля в клетках
}


std::string Ship::getImageFile() // название файла спрайта корабля
{
	return m_imagefile;
}

void Ship::setStartMousePos(sf::Vector2f mousePos) // установить стартовую позицию мыши
{
	m_startMousePos = mousePos;
}