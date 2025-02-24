#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <list>

#include "constants.h"
#include "battlemenu.h"
#include "infomenu.h"
#include "network.h"
#include "preparationcellularfield.h"
#include "menu.h"
#include "button.h"


class PreparationForBattleMenu : public Menu
{
public:
    PreparationForBattleMenu(sf::TcpSocket* socket);
    ~PreparationForBattleMenu();

	void drawMenu(sf::RenderWindow& window);

    void getStartMenuStatus(bool status); // ���������� ������  �������� � ��������� ����
    void setBattleModeStatus(bool status); // ���������� ������ �������� � ���� �����

    bool getBattleModeStatus(); // ������� ������ �������� � ���� �����
    bool getStartMenuStatus(); // ������� ������ �������� � ��������� ����

    std::vector<Ship*> getShipsOnTheTableVector(); // ������� ������ �������� ����������� �� �����

    sf::Vector2f getCellularFieldPosition(); // ������� ������� ����


private:
    bool checkDuplicate(std::vector<Ship*>& shipsVector, Ship* ship); // ��������� ���������� ������� � �������
    void mouseButtonPressedEvent(const sf::Event& event, sf::RenderWindow& window); // ��������� ������� ������� ������ ����
    void mouseButtonReleasedEvent(const sf::Event& event, CellularField& cellularfield); // ��������� ������� ���������� ������ ����
    void mouseMoveEvent(CellularField& cellularfield, sf::RenderWindow& window); // ��������� ������� �������� ����
    void dataProcessing(sf::Packet packet); // ��������� ������ � �������

    void startBattleMode(); // ������ ����� �����

    bool m_isBattleModeEnabled = false; // ������� �� ����� �����
    bool m_isWaitingModeEnabled = false; // ������� �� ����� ��������

    InfoMenu m_waiting_Menu = InfoMenu(WAITING_OPPONENT_PREPARATION_STRING); // ���� ��������

    PreparationCellularField *m_preparation�ellularField; // ���� ����������� ��������
    Button *m_goToStartMenu_Button; // ������ ��������� � ������� ����
    Button *m_goToBattleMenu_Button; // ������ �����

    std::vector<Ship*> m_shipsvector = { new Ship(800, 100, CELL_SIZE * 4, CELL_SIZE, CELLS4_SHIP, true),
                                new Ship(800, 200, CELL_SIZE * 3, CELL_SIZE, CELLS3_SHIP, true),
                                new Ship(1000, 200, CELL_SIZE * 3, CELL_SIZE, CELLS3_SHIP, true),
                                new Ship(800, 300, CELL_SIZE * 2, CELL_SIZE, CELLS2_SHIP, true),
                                new Ship(950, 300, CELL_SIZE * 2, CELL_SIZE, CELLS2_SHIP, true),
                                new Ship(1100, 300, CELL_SIZE * 2, CELL_SIZE, CELLS2_SHIP, true),
                                new Ship(800, 400, CELL_SIZE, CELL_SIZE, CELLS1_SHIP, true),
                                new Ship(900, 400, CELL_SIZE, CELL_SIZE, CELLS1_SHIP, true),
                                new Ship(1000, 400, CELL_SIZE, CELL_SIZE, CELLS1_SHIP, true),
                                new Ship(1100, 400, CELL_SIZE, CELL_SIZE, CELLS1_SHIP, true) };


    std::vector<Ship*> m_shipsOnTheTableVector; // ������ �������� �� �����

    Ship* m_currentMovableShip = nullptr; // ������� ������������� �������

    std::wstring m_opponentName; // ��� ����������
};

PreparationForBattleMenu::PreparationForBattleMenu(sf::TcpSocket* socket): Menu(BG_PREPARATION_IMAGE, socket)
{
    m_preparation�ellularField = new PreparationCellularField(CELLULARFIELD_X, CELLULARFIELD_Y);
    m_goToStartMenu_Button = new  Button(GO_TO_START_MENU_X, GO_TO_START_MENU_Y, GO_TO_START_MENU_WIDTH, GO_TO_START_MENU_HEIGHT, TO_START_MENU_STRING);
    m_goToBattleMenu_Button = new Button(GO_TO_BATTLE_MENU_X, GO_TO_BATTLE_MENU_Y, GO_TO_BATTLE_MENU_WIDTH, GO_TO_BATTLE_MENU_HEIGHT, BATTLE_TITLE);
}

PreparationForBattleMenu::~PreparationForBattleMenu()
{
    delete m_preparation�ellularField;
    delete m_goToStartMenu_Button;
    delete m_goToBattleMenu_Button;
}

void PreparationForBattleMenu::startBattleMode() // ������ ����� �����
{
    if (m_shipsOnTheTableVector.size() == SHIPS_CNT) // ��� ������� �����������?
    {
        int shipsParams[SHIPS_PARAMS_CNT * SHIPS_CNT];
        int i = 0;
        for (int j = 0; j < SHIPS_CNT; j++) // ������ �� ���� ��������
        {
            // ��������� �������
            int c_x = ((m_shipsOnTheTableVector[j])->getPos().x - m_preparation�ellularField->getPosition().x) / CELL_SIZE;
            int c_y = ((m_shipsOnTheTableVector[j])->getPos().y - m_preparation�ellularField->getPosition().y) / CELL_SIZE;
            int c_w =(m_shipsOnTheTableVector[j])->getSize().x / CELL_SIZE;
            int c_h = (m_shipsOnTheTableVector[j])->getSize().y / CELL_SIZE;

            // ������ ���������� � ������
            shipsParams[i] = c_x;
            shipsParams[i + 1] = c_y;
            shipsParams[i + 2] = c_w;
            shipsParams[i + 3] = c_h;
            i += SHIPS_PARAMS_CNT;
        }
        m_network->sendToServer(ServerCommands::readyToBattle, SHIPS_PARAMS_CNT * SHIPS_CNT, shipsParams); // �������� ������� ���������� � ����� � ������ ��������
    }
    else
        m_goToBattleMenu_Button->setErrorText(NOT_ALL_SHIPS_ON_THE_TABLE_ERROR); // ����� ������, ��� �� ��� ������� �����������
}


bool PreparationForBattleMenu::checkDuplicate(std::vector<Ship*>& shipsVector, Ship* ship) // ��������� ���������� ������� � �������
{
    bool isDuplicate = false; // ��� ��������
    if (shipsVector.size() != 0) // ���� ������ �� ������
    {
        int i = 0;
        while (!isDuplicate && (i < shipsVector.size())) // ������ �� ���� �������
        {
            if (shipsVector[i] == ship) // ���� ������� ����������
                isDuplicate = true; // ���� ������
            i++;
        }
    }
    return isDuplicate;
}

void PreparationForBattleMenu::dataProcessing(sf::Packet packet) // ��������� ������ � �������
{
    int command;
    packet >> command; // ��������� ������� �� ������
    switch (command)
    {
    case (ServerCommands::onBattleMode): // ������� ������ �����
    {
        packet >> m_opponentName; // ��������� ����� ���������� �� ������
        std::wcout << L"��� ����������: " << m_opponentName << std::endl;
        m_isBattleModeEnabled = true; // ������� ����� �����
        break;
    }
    case (ServerCommands::onWaitingMode): // ������� ��������
    {
        m_isWaitingModeEnabled = true; // ������� ����� ��������
        break;
    }
    default:
        break;
    }
}

void PreparationForBattleMenu::mouseButtonPressedEvent(const sf::Event& event, sf::RenderWindow& window) // ��������� ������� ������� ������ ����
{
    if (event.key.code == sf::Mouse::Left) // ���� ������ ����� ������
    {
        // �������� �� ������� �� ��������
        for (int i = 0; i < m_shipsvector.size(); i++)
        {
            sf::FloatRect shipRect = (m_shipsvector[i])->getSprite()->getGlobalBounds(); // ������������� �������
            sf::FloatRect mouseRect = sf::FloatRect(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 1, 1); // ������������� �������
            if (shipRect.intersects(mouseRect)) // ������ ���� �������� �� �������?
            {
                m_currentMovableShip = m_shipsvector[i]; // ������� ������������� �������

                int j = 0;
                bool isDeleted = false; // ���� �� ��������
                while (j < m_shipsOnTheTableVector.size() && !isDeleted) // ������ �� ���� ��������
                {
                    if (m_shipsOnTheTableVector[j] == m_currentMovableShip)
                    {
                        m_shipsOnTheTableVector.erase(m_shipsOnTheTableVector.begin() + j);
                        isDeleted = true;
                    }
                    j++;
                }
                m_shipsvector[i]->setStartMousePos(sf::Vector2f(sf::Mouse::getPosition()));
            }
        }
    }
}

void PreparationForBattleMenu::mouseButtonReleasedEvent(const sf::Event& event, CellularField& cellularfield)  // ��������� ������� ���������� ������ ����
{ 
    if (event.key.code == sf::Mouse::Left) // ���� �������� ����� ������
    {
        if (m_currentMovableShip != nullptr) // ���� ������� ������������� ������� ����������
        {
            float x, y;
            if (m_preparation�ellularField->getCanPutStatus()) // ���� ������� ����� ���������
            {
                x = m_preparation�ellularField->getRectPos().x; // ����� ������� x �������
                y = m_preparation�ellularField->getRectPos().y; // ����� ������� y �������
                if (!checkDuplicate(m_shipsOnTheTableVector, m_currentMovableShip)) // ���� ��� �� ����� �������
                    m_shipsOnTheTableVector.push_back(m_currentMovableShip); // ���������� ������� � ������ �������� �� �����
            }
            else
            {
                x = m_currentMovableShip->getStartPos().x; // ����� ������� x �������
                y = m_currentMovableShip->getStartPos().y; // ����� ������� y �������
            }


            m_currentMovableShip->getSprite()->setPosition(x, y); // ��������� ������� �������, �� �������� �� ����
            sf::Sprite sprite = *m_currentMovableShip->getSprite();
            m_preparation�ellularField->setIsDrawRectStatus(false);
            m_currentMovableShip = nullptr;
        }

        if (m_goToBattleMenu_Button->getPressedStatus()) // ���� ������ ������ �������� � ���� �����
        {
            startBattleMode(); // ������ ����� �����
            m_goToBattleMenu_Button->setPressedStatus(false);
        }
    }


}


void PreparationForBattleMenu::mouseMoveEvent(CellularField& cellularfield, sf::RenderWindow& window)
{
    if (m_currentMovableShip != nullptr)
    {
        m_currentMovableShip->update(sf::Vector2f(sf::Mouse::getPosition()));
        sf::FloatRect shipRect = m_currentMovableShip->getSprite()->getGlobalBounds();
        m_preparation�ellularField->checkCollision(shipRect.left, shipRect.top,
            shipRect.width, shipRect.height, &window);
    }
}

void PreparationForBattleMenu::drawMenu(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case (sf::Event::Closed): // ������� �������� ����
            window.close();
            break;
        case (sf::Event::MouseButtonPressed):
        {
            mouseButtonPressedEvent(event, window);
            break;
        }
        case (sf::Event::MouseButtonReleased):
        {
            mouseButtonReleasedEvent(event, *m_preparation�ellularField);
            break;
        }
        case (sf::Event::MouseMoved):
        {
            mouseMoveEvent(*m_preparation�ellularField, window);
            break;
        }
        case (sf::Event::KeyReleased):
        {
            if (m_currentMovableShip != nullptr)
            {
                if (event.key.code == sf::Keyboard::R)
                    m_currentMovableShip->rotate();
            }
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

    if (m_isWaitingModeEnabled)
    {
        m_waiting_Menu.drawMenu(window);
    }
    else
    {
        window.draw(m_backgroundSprite);
        m_preparation�ellularField->drawRectangle(&window, m_shipsOnTheTableVector);
        m_preparation�ellularField->drawField(&window);
        for (int i = 0; i < m_shipsvector.size(); i++)
        {
            if (m_shipsvector[i] != nullptr)
                window.draw(*(m_shipsvector[i]->getSprite()));
            else
                std::cout << "������ �������" << std::endl;
        }
        m_goToStartMenu_Button->drawButton(window);
        m_goToBattleMenu_Button->drawButton(window);
    }
    
}

bool PreparationForBattleMenu::getBattleModeStatus()
{
    return m_isBattleModeEnabled;
}

void PreparationForBattleMenu::setBattleModeStatus(bool status)
{
    m_isBattleModeEnabled = status;
}

std::vector<Ship*> PreparationForBattleMenu::getShipsOnTheTableVector()
{
    return m_shipsOnTheTableVector;
}

sf::Vector2f PreparationForBattleMenu::getCellularFieldPosition()
{
    return m_preparation�ellularField->getPosition();
}

bool PreparationForBattleMenu::getStartMenuStatus()
{
    if (m_goToStartMenu_Button->getPressedStatus() || m_waiting_Menu.getButton()->getPressedStatus())
        return true;
    return false;
}