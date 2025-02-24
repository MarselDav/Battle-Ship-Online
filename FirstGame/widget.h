#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "constants.h"

class Widget : public sf::RectangleShape
{
public:
	Widget(float x, float y, float width, float height);

	void setWidgetColor(sf::Color color);
	void setTitleColor(sf::Color color);
	void setBorderColor(sf::Color color);
	void setHoverBorderColor(sf::Color color);
	void setTitleFontSize(int size);
	void setErrorFontSize(int size);

	void setErrorText(std::wstring errorText);
	void clearErrorText();

	void drawContent(sf::RenderWindow& window);


protected:
	sf::Text title;
	sf::Text error;

	sf::Color widgetColor = DEFAULT_WIDGET_COLOR;
	sf::Color titleColor = DEFAULT_TITLE_COLOR;
	sf::Color borderColor = DEFAULT_BORDER_COLOR;
	sf::Color hoverBorderColor = DEFAULT_HOVER_BORDER_COLOR;
};

Widget::Widget(float x, float y, float width, float height)
{
	title.setFont(font); // Используем статически загруженный шрифт
	title.setCharacterSize(DEFAULT_TITLE_CHARACTER_SIZE);
	setTitleColor(titleColor);

	error.setFont(font);
	error.setPosition(x, y + height);
	title.setCharacterSize(DEFAULT_ERROR_CHARACTER_SIZE);
	error.setFillColor(ERROR_COLOR);

	setWidgetColor(widgetColor);
	this->setPosition(x, y);
	this->setSize(sf::Vector2f(width, height));
}

void Widget::drawContent(sf::RenderWindow &window)
{
	window.draw(*this);
	window.draw(title);
	window.draw(error);
}

void Widget::setErrorText(std::wstring errorText)
{
	error.setString(errorText);
}

void Widget::clearErrorText()
{
	error.setString("");
}

void Widget::setTitleFontSize(int size)
{
	title.setCharacterSize(size);
}

void Widget::setErrorFontSize(int size)
{
	error.setCharacterSize(size);
}

void Widget::setWidgetColor(sf::Color color)
{
	this->setFillColor(color);
}

void Widget::setTitleColor(sf::Color color)
{
	title.setFillColor(color);
}

void Widget::setBorderColor(sf::Color color)
{
	borderColor = color;
}

void Widget::setHoverBorderColor(sf::Color color)
{
	hoverBorderColor = color;
}