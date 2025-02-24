#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <list>

#include "constants.h"
#include "infomenu.h"
#include "cellularfield.h"
#include "menu.h"


class BattleMenu : public Menu
{
public:
    void drawMenu(sf::RenderWindow& window);

    BattleMenu(sf::TcpSocket* socket);

    void setNames(std::wstring playerNameString, std::wstring opponentNameString);
    void setShipsOnTheTableVector(std::vector<Ship*> shipsOnTheTable, sf::Vector2f cellularFieldPos);
    void setStartMenuButtonPressed(bool status);

    bool getStartMenuStatus();
    std::vector<Ship*> getShipsOnTheTableVector();

private:
    void dataProcessing(sf::Packet packet);
    void shotProcessing(sf::Packet packet, CellularField& cellularField, std::string imageFile);

    std::vector<Ship*> m_shipsOnTheTablesVector;

    std::vector<Object*> m_shotsObjectsVector;

    sf::TcpSocket* m_client;
    NetWork* m_network;

    CellularField m_playerCellularfield = CellularField(PLAYER_CELLULARFIELD_X, PLAYER_CELLULARFIELD_Y);
    CellularField m_enemyCellularfield = CellularField(OPPONENT_CELLULARFIELD_X, OPPONENT_CELLULARFIELD_Y);

    InfoMenu endGameMenu;
    Button* m_goToStartMenu_Button;

    sf::Image m_greenArrowImage;
    sf::Texture m_greenArrowTexture;
    sf::Image m_redArrowImage;
    sf::Texture m_redArrowTexture;
    sf::Sprite m_greenArrow;
    sf::Sprite m_redArrow;

    sf::Text m_playerName;
    sf::Text m_opponentName;

    bool isMyTurn = false;
    bool isGameEnd = false;
};

BattleMenu::BattleMenu(sf::TcpSocket* socket) : Menu(BG_BATTLE_IMAGE, socket)
{
    m_client = socket;
    m_network = new NetWork(socket);
    m_goToStartMenu_Button = new Button(GO_TO_START_MENU_X, GO_TO_START_MENU_Y, GO_TO_START_MENU_WIDTH, GO_TO_START_MENU_HEIGHT, TO_START_MENU_STRING);
    m_playerName.setFont(font);
    m_playerName.setFillColor(DEFAULT_TITLE_COLOR);
    m_playerName.setPosition(PLAYER_NAME_X, PLAYER_NAME_Y);
    m_opponentName.setFont(font);
    m_opponentName.setFillColor(DEFAULT_TITLE_COLOR);
    m_opponentName.setPosition(OPPONENT_NAME_X, OPPONENT_NAME_Y);

    m_greenArrowImage.loadFromFile(GREEN_ARROW_IMAGE);
    m_greenArrowTexture.loadFromImage(m_greenArrowImage);

    m_redArrowImage.loadFromFile(RED_ARROW_IMAGE);
    m_redArrowTexture.loadFromImage(m_redArrowImage);

    m_greenArrow.setScale(ARROW_SCALE_X, ARROW_SCALE_Y);
    m_greenArrow.setPosition(ARROW_X, ARROW_Y);
    m_greenArrow.setTexture(m_greenArrowTexture);

    m_redArrow.setScale(ARROW_SCALE_X, ARROW_SCALE_Y);
    m_redArrow.setPosition(ARROW_X, ARROW_Y);
    m_redArrow.setTexture(m_redArrowTexture);
}

void BattleMenu::shotProcessing(sf::Packet packet, CellularField& cellularField, std::string imageFile)
{
    int c_x, c_y;
    packet >> c_x;
    packet >> c_y;
    int x = c_x * CELL_SIZE + cellularField.getPosition().x;
    int y = c_y * CELL_SIZE + cellularField.getPosition().y;
    m_shotsObjectsVector.push_back(new Object(x, y, CELL_SIZE, CELL_SIZE, imageFile));
}

void BattleMenu::dataProcessing(sf::Packet packet)
{
    int command;
    packet >> command;
    switch (command)
    {
    case (ServerCommands::myTurn):
    {
        isMyTurn = true;
        break;
    }
    case (ServerCommands::opponentTurn):
    {
        isMyTurn = false;
        break;
    }
    case (ServerCommands::myHit):
    {
        shotProcessing(packet, m_enemyCellularfield, HIT_IMAGE);
        break;
    }
    case (ServerCommands::myMiss):
    {
        shotProcessing(packet, m_enemyCellularfield, MISS_IMAGE);
        break;
    }
    case (ServerCommands::opponentHit):
    {
        shotProcessing(packet, m_playerCellularfield, HIT_IMAGE);
        break;
    }
    case (ServerCommands::opponentMiss):
    {
        shotProcessing(packet, m_playerCellularfield, MISS_IMAGE);
        break;
    }
    case (ServerCommands::destroyed):
    {
        int c_x, c_y, c_firstSizeParam, c_secondSizeParam, c_w, c_h;
        packet >> c_x;
        packet >> c_y;
        packet >> c_firstSizeParam;
        packet >> c_secondSizeParam;

        c_w = c_firstSizeParam > c_secondSizeParam ? c_firstSizeParam : c_secondSizeParam;
        c_h = c_firstSizeParam < c_secondSizeParam ? c_firstSizeParam : c_secondSizeParam;
        int x = c_x * CELL_SIZE + m_enemyCellularfield.getPosition().x;
        int y = c_y * CELL_SIZE + m_enemyCellularfield.getPosition().y;

        char shipImageFileChar[50];
        sprintf_s(shipImageFileChar, CELL_SHIP_FORMAT_STRING, c_w);
        std::string shipImageFileString = shipImageFileChar;

        Ship* enemyShip = new Ship(x, y, c_w * CELL_SIZE, c_h * CELL_SIZE, shipImageFileString, false);

        if (c_secondSizeParam > c_firstSizeParam)
            enemyShip->rotate();
        m_shipsOnTheTablesVector.push_back(enemyShip);
        break;
    }
    case (ServerCommands::win):
    {
        std::cout << ServerCommands::win << std::endl;
        endGameMenu.setText(WIN_STRING);
        isGameEnd = true;
        break;
    }
    case (ServerCommands::lose):
    {
        std::cout << "lose" << std::endl;
        endGameMenu.setText(LOSE_STRING);
        isGameEnd = true;
        break;
    }
    default:
        break;
    }
}

void BattleMenu::setNames(std::wstring playerNameString, std::wstring opponentNameString)
{
    m_playerName.setString(playerNameString);
    m_opponentName.setString(opponentNameString);
}

void BattleMenu::setShipsOnTheTableVector(std::vector<Ship*> shipsOnTheTable, sf::Vector2f cellularFieldPos)
{
    for (int i = 0; i < shipsOnTheTable.size(); i++)
    {
        sf::Vector2i positionInCells = shipsOnTheTable[i]->getPositionInCells(cellularFieldPos);
        int x = positionInCells.x * CELL_SIZE + m_playerCellularfield.getPosition().x;
        int y = positionInCells.y * CELL_SIZE + m_playerCellularfield.getPosition().y;
        Ship *ship = shipsOnTheTable[i];
        ship->getSprite()->setPosition(x, y);
        m_shipsOnTheTablesVector.push_back(ship);
    }
}


void BattleMenu::drawMenu(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case (sf::Event::Closed): // событие закрытия окна
        {
            window.close();
            break;
        }
        case (sf::Event::MouseButtonReleased):
        {
            if (event.key.code == sf::Mouse::Left)
            {
                if (m_enemyCellularfield.getRect().intersects(
                    sf::FloatRect(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 1, 1)))
                {
                    if (isMyTurn && !isGameEnd)
                    {
                        sf::Vector2i cell_number = m_enemyCellularfield.getCellNumber(sf::Mouse::getPosition(window));
                        int shotCordsInCell[] = { cell_number.x, cell_number.y };

                        //std::cout << "попал: " << argv[0] << " " << argv[1] << std::endl;
                        if (m_network->sendToServer(ServerCommands::shot, sizeof(shotCordsInCell) / sizeof(int), shotCordsInCell) == sf::Socket::Done)
                        {
                            std::cout << "ход: " << shotCordsInCell[0] << " " << shotCordsInCell[1] << std::endl;
                            //isMyTurn = false;
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }

    sf::Packet packet;
    if (m_client->receive(packet) == sf::Socket::Done)
    {
        dataProcessing(packet);
    }
    if (!isGameEnd)
    {
        window.draw(m_backgroundSprite);
        m_playerCellularfield.drawField(&window);
        m_enemyCellularfield.drawField(&window);
        window.draw(m_playerName);
        window.draw(m_opponentName);
        if (isMyTurn)
            window.draw(m_greenArrow);
        else
            window.draw(m_redArrow);

        // отрисовка кораблей
        for (int i = 0; i < m_shipsOnTheTablesVector.size(); i++)
        {
            window.draw(*(m_shipsOnTheTablesVector[i]->getSprite()));
        }

        // отрисовка промахов
        for (int i = 0; i < m_shotsObjectsVector.size(); i++)
        {
            window.draw(*(m_shotsObjectsVector[i]->getSprite()));
        }
        m_goToStartMenu_Button->drawButton(window);
    }
    else
    {
        endGameMenu.drawMenu(window);
    }

}

std::vector<Ship*> BattleMenu::getShipsOnTheTableVector()
{
    return m_shipsOnTheTablesVector;
}


bool BattleMenu::getStartMenuStatus()
{
    if (m_goToStartMenu_Button->getPressedStatus() || endGameMenu.getButton()->getPressedStatus())
        return true;
    return false;
}

void BattleMenu::setStartMenuButtonPressed(bool status)
{
    m_goToStartMenu_Button->setPressedStatus(status);
}