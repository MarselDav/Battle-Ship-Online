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

	void update(sf::Vector2f mousePos); // ���������� ������� �������
	void rotate(); // �������� �������

	void setStartMousePos(sf::Vector2f mousePos); // ���������� ��������� ������� ����

	sf::Vector2i getPositionInCells(sf::Vector2f cellularFieldPos); // ������� ������� ������� � �������
	std::string getImageFile(); // ������� �������� ����� ������� �������

private:
	sf::Vector2f m_startMousePos; // ��������� ������� ����
	std::string m_imagefile; // �������� ����� ������� �������
	bool m_isMovable; // ���� ����� �� ������� �������
};

Ship::Ship(float x, float y, float width, float height, std::string imagefile, bool isMovable) : Object(x, y, width, height, imagefile)
{
	m_isMovable = isMovable; // ���� ����� �� ������� �������
	m_imagefile = imagefile; // �������� ����� ������� �������
}

void Ship::rotate() // �������� �������
{
	if (sprite.getRotation() == ROTATE_ANGLE) // ���� ���� ������� ����� 90 ��������
	{
		sprite.setRotation(0); // ������� � �������� ���������
		sprite.setOrigin(0, 0); // ���������� ����� ������ ���������� ������� �� ���������
	}
	else
	{
		sprite.setRotation(ROTATE_ANGLE); // ��������� �� ���� 90 ��������
		sprite.setOrigin(0, sprite.getGlobalBounds().width); // ���������� ����� ����� ������ ���������� �������, ���������� �� ������ �������
	}
}

void Ship::update(sf::Vector2f mousePos) // ���������� ������� �������
{
	if (m_isMovable) // ���� ������� ����� ���������
	{
		sprite.setPosition(sprite.getPosition().x + mousePos.x - m_startMousePos.x, // �������� ������� ������� �� ������� ����
			sprite.getPosition().y + mousePos.y - m_startMousePos.y);
		m_startMousePos = mousePos; // �������� ��������� ������� ���� �� �������
	}
}

sf::Vector2i Ship::getPositionInCells(sf::Vector2f cellularFieldPos) // ������� ������� ������� � �������
{
	return sf::Vector2i((sprite.getPosition().x - cellularFieldPos.x) / CELL_SIZE ,
		(sprite.getPosition().y - cellularFieldPos.y) / CELL_SIZE); // ������� ������� ������� � �������
}


std::string Ship::getImageFile() // �������� ����� ������� �������
{
	return m_imagefile;
}

void Ship::setStartMousePos(sf::Vector2f mousePos) // ���������� ��������� ������� ����
{
	m_startMousePos = mousePos;
}