#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <list>

#include "constants.h"
#include "preparationforbattlemenu.h"
#include "infomenu.h"
#include "lineedit.h"
#include "button.h"

class StartMenu : public Menu
{
public:
    StartMenu(sf::TcpSocket* socket);
    ~StartMenu();

    void drawMenu(sf::RenderWindow& window); // ��������� ����

    // ������
    void resetFlags(); // �������� ����� �� ���������
    void setPreparationModeStatus(bool status); // ���������� ������ �������� ���� ����������
    void setDisconnectedStatus(bool status); // ���������� ������ ���������� �� �������

    // ������
    bool getDisconnectedStatus(); // ������� ���������� �� ������ ���������� �� �������
    bool getPreparationModeStatus(); // ������� ������� �� ����� ����������

    std::wstring getPlayerName(); // ������� ��� ������
    std::wstring getOpponentName();  // ������� ��� ����������

private:
    void initializeButtonData();  // ������������� ������ ������
    void connectToServer(); // ������������ � �������
    void dataProcessing(sf::Packet packet); // ��������� ������ � �������

    std::wstring m_playerName; // ��� ������
    std::wstring m_opponentName; // ��� ����������

    // ������� start menu
    Button* m_playOnline_Button; // ������ ������ ������
    LineEdit* m_playerName_Input; // ���� ����� �����

    // waiting menu
    InfoMenu m_waiting_Menu = InfoMenu(WAITING_OPPONENT_CONNECT_STRING); //���� �������� ����������

    bool m_isPreparationModeEnabled; // ������� �� ��� ���������� � �����
    bool m_isWaitingModeEnabled; // ������� �� ��� �������� ����������
};

StartMenu::StartMenu(sf::TcpSocket* socket) : Menu(BG_START_IMAGE, socket)
{
    resetFlags(); // �������� ����� �� ���������
    initializeButtonData(); // ������������� ������ ������
}

StartMenu::~StartMenu()
{
    delete m_playOnline_Button;
    delete m_playerName_Input;
}

void StartMenu::initializeButtonData() // ������������� ������ ������
{
    int windowCenterX = (WIDTH - PLAY_BUTTON_WIDTH) / 2; // ����� ���� x
    int windowCenterY = (HEIGHT - PLAY_BUTTON_HEIGHT) / 2; // ����� ���� y

    m_playOnline_Button = new Button(windowCenterX, windowCenterY + PLAY_BUTTON_OFFSET,
        PLAY_BUTTON_WIDTH, PLAY_BUTTON_HEIGHT, PLAY_ONLINE_BUTTON_STRING); //  ������������� �������� ������ "������ ������"

    m_playerName_Input = new LineEdit(windowCenterX, windowCenterY + NAMEINPUT_OFFSET, NAME_INPUT_WIDTH, NAME_INPUT_HEIGHT); //  ������������� �������� ���� "���� �����"

    m_playerName_Input->setFillColor(LINE_EDIT_BG); // ��������� ���� ���� ����� �����

}

void StartMenu::connectToServer() // ����������� � �������
{
    sf::Socket::Status connect_status = m_network->connectToServer(); // ����������� � �������
    if (connect_status != sf::Socket::Done) // ���� ��� ����������� ��������� ������
    {
        m_playOnline_Button->setErrorText(SERVER_CONNECTING_ERROR);  // ����� ������ ��� �������
    }
    else
    {
        std::wstring str[] = { m_playerName_Input->getWText() };
        m_playerName = m_playerName_Input->getWText(); // ��������� ����� ������������ �� ���� ��� ����� ������

        sf::Socket::Status send_status = m_network->sendToServer(ServerCommands::setPlayerName, 1, str); // �������� ����� ������������ �� ������
    }
    m_client->setBlocking(false); // ��������� ����� ���������� ������
}
 
void StartMenu::dataProcessing(sf::Packet packet) // ��������� ������ � �������
{
    int command;
    packet >> command; // ��������� �������� �� ������ � �������
    switch (command)
    {
    case (ServerCommands::onPreparationMode): // ���� ������ �������� �� ��������� ����������� ��������
    {
        packet >> m_opponentName; // ��������� ����� ��������� �� ������
        m_isPreparationModeEnabled = true; // ��������� ���� ����������� ��������
        break;
    }
    case (ServerCommands::onWaitingMode):  // ���� ������ �������� �� ��������� ���� ��������
    {
        m_isWaitingModeEnabled = true; // ��������� ���� ��������
        break;
    }
    default:
        break;
    }
}

void StartMenu::drawMenu(sf::RenderWindow& window) // ��������� ���������� ����
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case (sf::Event::Closed): // ������� �������� ����
            window.close();
            break;
        case (sf::Event::TextEntered): // ������� ����� ������
        {
            m_playerName_Input->editText(event); // �������������� �����
            break;
        }
        case (sf::Event::KeyPressed): // ������� ������� ������� ����������
        {
            m_playerName_Input->keyPressed(event); // ��������� ������� ������� ������� ���������� � ���� �����
            break;
        }
        case (sf::Event::MouseButtonReleased): // ������� ���������� ������ ����
            if (m_playOnline_Button->getPressedStatus())
            {
                if (m_playerName_Input->getWText().empty()) // ���� ������������ �� ��� ���
                {
                    m_playerName_Input->setErrorText(EMPTY_NAME_ERROR); // ����� ������
                }
                else
                {
                    m_playerName_Input->clearErrorText(); // ������� ������ ������
                    connectToServer(); // ����������� � �������
                }
                m_playOnline_Button->setPressedStatus(false);
            }
            if (m_waiting_Menu.getButton()->getPressedStatus()) // ���� ������ "� ������� ����" ���� �������� ������
            {
                m_isPreparationModeEnabled = false; // ��������� ��� ����������
                m_isWaitingModeEnabled = false; // ��������� ��� ��������
                m_isSocketDisconnected = true; // �������� ���� ���������� �� �������
                m_waiting_Menu.getButton()->setPressedStatus(false);
            }
            break;
        default:
            break;
        }

    }

    sf::Packet packet;
    if (m_client->receive(packet) == sf::Socket::Done) // ������� ������� ����� ���������� ?
    {
        dataProcessing(packet); // ��������� ���������� � ������� ����������
    }

    window.draw(m_backgroundSprite); // ��������� ������� ����
    if (m_isWaitingModeEnabled) // ��� �������� ������� ?
    {
        m_waiting_Menu.drawMenu(window); // ��������� ���� ��������
    }
    else
    {
        m_playOnline_Button->drawButton(window); // ��������� ������ "������ ������"
        m_playerName_Input->drawLineEdit(window); // ��������� ���� ����� �����
    }
}

// �������� ����� �� ���������
void StartMenu::resetFlags()
{
    m_isPreparationModeEnabled = false; // ������� �� ��� ���������� � �����
    m_isWaitingModeEnabled = false; // ������� �� ��� �������� ����������
}

// ���������� ������ ���������� �� �������
void StartMenu::setDisconnectedStatus(bool status)
{
    m_isSocketDisconnected = status;
}

// ���������� ������ �������� ���� ����������
void StartMenu::setPreparationModeStatus(bool status)
{
    m_isPreparationModeEnabled = status;
}

// ������� ��� ������
std::wstring StartMenu::getPlayerName()
{
    return m_playerName;
}

// ������� ��� ����������
std::wstring StartMenu::getOpponentName()
{
    return m_opponentName;
}

// ������� ������� �� ����� ����������
bool StartMenu::getPreparationModeStatus()
{
    return m_isPreparationModeEnabled;
}

// ������� ���������� �� ������ ���������� �� �������
bool StartMenu::getDisconnectedStatus()
{
    return m_isSocketDisconnected;
}