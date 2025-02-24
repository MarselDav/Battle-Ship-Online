#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <list>

#include "constants.h"
#include "menu.h"
#include "button.h"


class InfoMenu : public Menu
{
public:
    InfoMenu(std::wstring string = L"");

    void drawMenu(sf::RenderWindow& window);

    void setText(std::wstring string);
    Button* getButton();

private:
    sf::Text m_text;
    Button* m_button;

};

InfoMenu::InfoMenu(std::wstring string) : Menu(BG_WAITING_IMAGE)
{
    int windowCenterX = (WIDTH - PLAY_BUTTON_WIDTH) / 2;
    int windowCenterY = (HEIGHT - PLAY_BUTTON_HEIGHT) / 2;

    m_button = new Button(GOBACK_MENU_BUTTON_X, GOBACK_MENU_BUTTON_Y,
        GOBACK_BUTTON_WIDTH, GOBACK_BUTTON_HEIGHT, TO_START_MENU_STRING);

    m_text.setFont(font);
    m_text.setFillColor(DEFAULT_TITLE_COLOR);
    m_text.setString(string);
    m_text.setPosition(windowCenterX, windowCenterY);
}


void InfoMenu::drawMenu(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case (sf::Event::Closed): // событие закрытия окна
            window.close();
            break;
        default:
            break;
        }
    }

    window.draw(m_backgroundSprite);
    window.draw(m_text);
    m_button->drawButton(window);
}

void InfoMenu::setText(std::wstring string)
{
    m_text.setString(string);
}


Button* InfoMenu::getButton()
{
    return m_button;
}
