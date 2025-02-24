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
    MenuState currentMenu = MenuState::Start_Menu; // текущее меню

    StartMenu* startmenu; // стартовое меню
    PreparationForBattleMenu* preparationmenu; // меню подготовки
    BattleMenu* battlemenu; // меню битвы

    Menus()
    {
        startmenu = new StartMenu(socket);
        preparationmenu = new PreparationForBattleMenu(socket);
        battlemenu = new BattleMenu(socket);
    }
};

void StartMenuStateProcessing(sf::RenderWindow &window, Menus& menus) // обработка состояния стартового меню
{
    menus.startmenu->drawMenu(window); // отрисовка стартового меню

    // если игрок отключается от сервера
    if (menus.startmenu->getDisconnectedStatus())
    {
        socket->disconnect(); // отключение от сервера
        delete socket; // освобождение памяти сокета
        socket = new sf::TcpSocket; // выделение памяти для сокета
        menus.startmenu->setNewSocket(socket); // передача нового сокета в стартовое меню
    }

    // если противник найден и игрок переходит в меню подготовки к битве
    if (menus.startmenu->getPreparationModeStatus())
    {
        menus.preparationmenu = new PreparationForBattleMenu(socket);  // выделение новой памяти для меню подготовки
        menus.currentMenu = MenuState::Preparation_Menu; // текущее мею - меню подготовки
        menus.startmenu->resetFlags(); // сбросить флаги до дефолтных
    }
}

void PreparationMenuStateProcessing(sf::RenderWindow& window, Menus& menus) // обработка состояния меню подготовки
{
    menus.preparationmenu->drawMenu(window); // отрисовка меню подготовки

    // если игрок выходит в стартовое меню
    if (menus.preparationmenu->getStartMenuStatus())
    {
        menus.startmenu->setDisconnectedStatus(true); // установить флажок отключение от сервера
        menus.currentMenu = MenuState::Start_Menu; // текущее меню - стартовое меню
    }
    // если игрок перемещается в меню битвы
    if (menus.preparationmenu->getBattleModeStatus())
    {
        menus.battlemenu = new BattleMenu(socket); // выделение новой памяти для меню битвы
        menus.battlemenu->setNames(menus.startmenu->getPlayerName(), menus.startmenu->getOpponentName()); // установка имени игроков
        // установить вектор кораблей для меню битвы
        menus.battlemenu->setShipsOnTheTableVector(menus.preparationmenu->getShipsOnTheTableVector(), menus.preparationmenu->getCellularFieldPosition());
        menus.currentMenu = MenuState::Battle_Menu; // текущее меню - меню битвы 
    }
}

void BattleMenuStateProcessing(sf::RenderWindow& window, Menus& menus) // обработка состояния меню битвы
{
    menus.battlemenu->drawMenu(window); // отрисовка меню битвы

    if (menus.battlemenu->getStartMenuStatus()) // если игрок выходит в стартовое меню
    {
        menus.startmenu->setDisconnectedStatus(true); // установить флажок отключение от сервера
        menus.currentMenu = MenuState::Start_Menu; // текущее меню - стартовое меню
    }
}

int main()
{
    setlocale(LC_ALL, "rus"); // установить текст консоли

    std::ofstream errorFile("error.log"); // открытие файла ошибок
    if (errorFile)
        std::cerr.rdbuf(errorFile.rdbuf()); // Перенаправление вывода std::cerr в файл ошибок
    else
        std::cerr << "Ошибка открытия файла для записи ошибок\n";

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), L"Морской бой"); // создание графического окна
    Menus menus; // структура с меню

    if (font.loadFromFile(fontName)) // загрузка файла шрифта
    {
        while (window.isOpen())	//цикл пока графическое окно не закрыто
        {
            window.clear(sf::Color::White); // очистка экрана
            switch (menus.currentMenu)
            {
            case (MenuState::Start_Menu): // если открыто стартовое меню
            {
                StartMenuStateProcessing(window, menus); // обработка состояния стартового меню
                break;
            }
            case (MenuState::Preparation_Menu): // если открыто меню подготовки
            {
                PreparationMenuStateProcessing(window, menus); // обработка состояния меню подготовки
                break;
            }
            case (MenuState::Battle_Menu): // если открыто меню битвы
            {
                BattleMenuStateProcessing(window, menus); // обработка состояния меню битвы
                break;
            }
            default:
                break;
            }
            window.display(); // отображение графики на экране
        }
    }
    else
    {
        std::cout << "Ошибка загрузки шрифта";
    }

    // очистка памяти указателей
    delete socket;
    delete menus.startmenu;
    delete menus.preparationmenu;
    delete menus.battlemenu;

    return 0;
}
