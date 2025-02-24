#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>

#include "constants.h"
#include "startmenu.h"
#include "preparationforbattlemenu.h"
#include "battlemenu.h"


sf::TcpSocket* socket = new sf::TcpSocket;

enum MenuState {
    Start_Menu,
    Preparation_Menu,
    Battle_Menu
};

struct Menus
{
    MenuState currentMenu = MenuState::Start_Menu; // ������� ����

    StartMenu* startmenu; // ��������� ����
    PreparationForBattleMenu* preparationmenu; // ���� ����������
    BattleMenu* battlemenu; // ���� �����

    Menus()
    {
        startmenu = new StartMenu(socket);
        preparationmenu = new PreparationForBattleMenu(socket);
        battlemenu = new BattleMenu(socket);
    }
};

void StartMenuStateProcessing(sf::RenderWindow &window, Menus& menus) // ��������� ��������� ���������� ����
{
    menus.startmenu->drawMenu(window); // ��������� ���������� ����

    // ���� ����� ����������� �� �������
    if (menus.startmenu->getDisconnectedStatus())
    {
        socket->disconnect(); // ���������� �� �������
        delete socket; // ������������ ������ ������
        socket = new sf::TcpSocket; // ��������� ������ ��� ������
        menus.startmenu->setNewSocket(socket); // �������� ������ ������ � ��������� ����
    }

    // ���� ��������� ������ � ����� ��������� � ���� ���������� � �����
    if (menus.startmenu->getPreparationModeStatus())
    {
        menus.preparationmenu = new PreparationForBattleMenu(socket);  // ��������� ����� ������ ��� ���� ����������
        menus.currentMenu = MenuState::Preparation_Menu; // ������� ��� - ���� ����������
        menus.startmenu->resetFlags(); // �������� ����� �� ���������
    }
}

void PreparationMenuStateProcessing(sf::RenderWindow& window, Menus& menus) // ��������� ��������� ���� ����������
{
    menus.preparationmenu->drawMenu(window); // ��������� ���� ����������

    // ���� ����� ������� � ��������� ����
    if (menus.preparationmenu->getStartMenuStatus())
    {
        menus.startmenu->setDisconnectedStatus(true); // ���������� ������ ���������� �� �������
        menus.currentMenu = MenuState::Start_Menu; // ������� ���� - ��������� ����
    }
    // ���� ����� ������������ � ���� �����
    if (menus.preparationmenu->getBattleModeStatus())
    {
        menus.battlemenu = new BattleMenu(socket); // ��������� ����� ������ ��� ���� �����
        menus.battlemenu->setNames(menus.startmenu->getPlayerName(), menus.startmenu->getOpponentName()); // ��������� ����� �������
        // ���������� ������ �������� ��� ���� �����
        menus.battlemenu->setShipsOnTheTableVector(menus.preparationmenu->getShipsOnTheTableVector(), menus.preparationmenu->getCellularFieldPosition());
        menus.currentMenu = MenuState::Battle_Menu; // ������� ���� - ���� ����� 
    }
}

void BattleMenuStateProcessing(sf::RenderWindow& window, Menus& menus) // ��������� ��������� ���� �����
{
    menus.battlemenu->drawMenu(window); // ��������� ���� �����

    if (menus.battlemenu->getStartMenuStatus()) // ���� ����� ������� � ��������� ����
    {
        menus.startmenu->setDisconnectedStatus(true); // ���������� ������ ���������� �� �������
        menus.currentMenu = MenuState::Start_Menu; // ������� ���� - ��������� ����
    }
}

int main()
{
    setlocale(LC_ALL, "rus"); // ���������� ����� �������

    std::ofstream errorFile("error.log"); // �������� ����� ������
    if (errorFile)
        std::cerr.rdbuf(errorFile.rdbuf()); // ��������������� ������ std::cerr � ���� ������
    else
        std::cerr << "������ �������� ����� ��� ������ ������\n";

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), L"������� ���"); // �������� ������������ ����
    Menus menus; // ��������� � ����

    if (font.loadFromFile(fontName)) // �������� ����� ������
    {
        while (window.isOpen())	//���� ���� ����������� ���� �� �������
        {
            window.clear(sf::Color::White); // ������� ������
            switch (menus.currentMenu)
            {
            case (MenuState::Start_Menu): // ���� ������� ��������� ����
            {
                StartMenuStateProcessing(window, menus); // ��������� ��������� ���������� ����
                break;
            }
            case (MenuState::Preparation_Menu): // ���� ������� ���� ����������
            {
                PreparationMenuStateProcessing(window, menus); // ��������� ��������� ���� ����������
                break;
            }
            case (MenuState::Battle_Menu): // ���� ������� ���� �����
            {
                BattleMenuStateProcessing(window, menus); // ��������� ��������� ���� �����
                break;
            }
            default:
                break;
            }
            window.display(); // ����������� ������� �� ������
        }
    }
    else
    {
        std::cout << "������ �������� ������";
    }

    // ������� ������ ����������
    delete socket;
    delete menus.startmenu;
    delete menus.preparationmenu;
    delete menus.battlemenu;

    return 0;
}
