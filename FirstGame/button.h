#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "widget.h"


class Button : public Widget
{
public:
	Button(float x, float y, float width, float height, std::wstring text);
	
	void drawButton(sf::RenderWindow& window);
	bool getPressedStatus();
	void setPressedStatus(bool status);


private:
	bool isPressed = false;

};

Button::Button(float x, float y, float width, float height, std::wstring text) : Widget(x, y, width, height)
{
	title.setString(text);
}


void Button::drawButton(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (this->getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1)))
	{
		this->setOutlineColor(borderColor);
		this->setOutlineThickness(OUTLINE_HOVER_THICKNESS);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			isPressed = true;
		}
	}
	else
	{
		this->setOutlineColor(hoverBorderColor);
		this->setOutlineThickness(OUTLINE_THICKNESS);
	}

	title.setPosition(
		this->getGlobalBounds().left + (this->getGlobalBounds().width -
		title.getLocalBounds().width) / 2,
		this->getGlobalBounds().top + (this->getGlobalBounds().height -
		title.getLocalBounds().height) / 2 - (title.getCharacterSize() - 
		title.getLocalBounds().height));

	drawContent(window);
}

void Button::setPressedStatus(bool status)
{
	isPressed = status;
}

bool Button::getPressedStatus()
{
	return isPressed;
}