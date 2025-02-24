#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <map>

#include "constants.h"
#include "widget.h"

const std::wstring PLACEHOLDER_TEXT = L"������� ���";
const sf::Color PLACEHOLDER_COLOR(230, 230, 230);
const sf::Color FONT_COLOR = sf::Color::Black;

std::map<std::string, std::wstring> W_LETTERS_MAP = {
	{"a", L"�"}, {"b", L"�"}, {"c", L"�"}, {"d", L"�"},
	{"e", L"�"}, {"f", L"�"}, {"g", L"�"}, {"h", L"�"},
	{"i", L"�"}, {"j", L"�"}, {"k", L"�"}, {"l", L"�"},
	{"m", L"�"}, {"n", L"�"}, {"o", L"�"}, {"p", L"�"},
	{"q", L"�"}, {"r", L"�"}, {"s", L"�"}, {"t", L"�"},
	{"u", L"�"}, {"v", L"�"}, {"w", L"�"}, {"x", L"�"},
	{"y", L"�"}, {"z", L"�"}, {"[", L"�"}, {"]", L"�"},
	{"'", L"�"},{";", L"�"}, {"`", L"�"}, {",", L"�"},
	{".", L"�"}, {"`", L"�"}, {"~", L"�"},
};
std::map<std::string, std::wstring>::iterator LETTERS_MAP_ITERATOR;

class LineEdit : public Widget
{
public:
	LineEdit(float x, float y, float width, float height);

	void editText(const sf::Event& event);
	void keyPressed(const sf::Event& event);
	void setSelectStatus(bool status);
	void drawLineEdit(sf::RenderWindow& window);
	std::wstring getWText();

private:
	std::wstring wtext;
	bool isSelect = false;

	void updateText(std::wstring str);
	bool checkKeyIsLetter(std::string key);
};

LineEdit::LineEdit(float x, float y, float width, float height) : Widget(x, y, width, height)
{
}

void LineEdit::setSelectStatus(bool status)
{
	isSelect = status;
}

bool LineEdit::checkKeyIsLetter(std::string key)
{
	LETTERS_MAP_ITERATOR = W_LETTERS_MAP.find(key);
	return LETTERS_MAP_ITERATOR != W_LETTERS_MAP.end();
}

void LineEdit::updateText(std::wstring str)
{
	title.setString(str);
	title.setPosition(this->getGlobalBounds().left + (this->getGlobalBounds().width - title.getLocalBounds().width) / 2,
		this->getGlobalBounds().top + (this->getGlobalBounds().height - title.getLocalBounds().height) / 2 - (title.getCharacterSize() - title.getLocalBounds().height));
}

void LineEdit::editText(const sf::Event& event)
{
	// �������������� ���� ������� � char
	char typedChar = static_cast<char>(event.text.unicode);
	std::string typedStr(1, typedChar);
	if (checkKeyIsLetter(typedStr) && isSelect)
	{
		if (wtext.size() < MAX_LINE_EDIT_SIMBOLS_CNT)
		{
			wtext += W_LETTERS_MAP[typedStr];
			updateText(wtext);
		}
	}

}

void LineEdit::keyPressed(const sf::Event& event)
{
	sf::Keyboard::Key pressedkey = event.key.code;
	if (pressedkey == sf::Keyboard::Enter)
	{
		isSelect = false;
	}
	else if (pressedkey == sf::Keyboard::BackSpace || pressedkey == sf::Keyboard::Delete)
	{
		if (!wtext.empty())
		{
			wtext.pop_back();
			updateText(wtext);
		}
	}
}

void LineEdit::drawLineEdit(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (this->getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1)))
	{
		this->setOutlineColor(hoverBorderColor);
		this->setOutlineThickness(OUTLINE_HOVER_THICKNESS);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			isSelect = true;
	}
	else
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			isSelect = false;
		this->setOutlineColor(borderColor);
		this->setOutlineThickness(OUTLINE_THICKNESS);
	}
	if (isSelect)
	{
		this->setOutlineThickness(OUTLINE_SELECT_THICKNESS);
	}
	if (wtext.empty())
	{
		updateText(PLACEHOLDER_TEXT);
		setTitleColor(PLACEHOLDER_COLOR);
	}
	else
	{
		setTitleColor(FONT_COLOR);
	}
	
	drawContent(window);
}

std::wstring LineEdit::getWText()
{
	return wtext;
}
