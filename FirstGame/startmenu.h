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

    void drawMenu(sf::RenderWindow& window); // отрисовка меню

    // сетеры
    void resetFlags(); // сбросить флаги до дефолтных
    void setPreparationModeStatus(bool status); // установить флажок открытия меню подготовки
    void setDisconnectedStatus(bool status); // установить флажок отключения от сервера

    // гетеры
    bool getDisconnectedStatus(); // вернуть установлен ли флажок отключения от сервера
    bool getPreparationModeStatus(); // вернуть включен ли режим подготовки

    std::wstring getPlayerName(); // вернуть имя игрока
    std::wstring getOpponentName();  // вернуть имя противника

private:
    void initializeButtonData();  // инициализация данных кнопок
    void connectToServer(); // подключиться к серверу
    void dataProcessing(sf::Packet packet); // обработка данных с сервера

    std::wstring m_playerName; // имя игрока
    std::wstring m_opponentName; // имя противника

    // виджеты start menu
    Button* m_playOnline_Button; // кнопка играть онлайн
    LineEdit* m_playerName_Input; // поле ввода имени

    // waiting menu
    InfoMenu m_waiting_Menu = InfoMenu(WAITING_OPPONENT_CONNECT_STRING); //меню ожидания противника

    bool m_isPreparationModeEnabled; // включен ли мод подготовки к битве
    bool m_isWaitingModeEnabled; // включен ли мод ожидания противника
};

StartMenu::StartMenu(sf::TcpSocket* socket) : Menu(BG_START_IMAGE, socket)
{
    resetFlags(); // сбросить флаги до дефолтных
    initializeButtonData(); // инициализация данных кнопок
}

StartMenu::~StartMenu()
{
    delete m_playOnline_Button;
    delete m_playerName_Input;
}

void StartMenu::initializeButtonData() // инициализация данных кнопок
{
    int windowCenterX = (WIDTH - PLAY_BUTTON_WIDTH) / 2; // центр окна x
    int windowCenterY = (HEIGHT - PLAY_BUTTON_HEIGHT) / 2; // центр окна y

    m_playOnline_Button = new Button(windowCenterX, windowCenterY + PLAY_BUTTON_OFFSET,
        PLAY_BUTTON_WIDTH, PLAY_BUTTON_HEIGHT, PLAY_ONLINE_BUTTON_STRING); //  инициализация размеров кнопки "играть онлайн"

    m_playerName_Input = new LineEdit(windowCenterX, windowCenterY + NAMEINPUT_OFFSET, NAME_INPUT_WIDTH, NAME_INPUT_HEIGHT); //  инициализация размеров поля "ввод имени"

    m_playerName_Input->setFillColor(LINE_EDIT_BG); // установка цвет поля ввода имени

}

void StartMenu::connectToServer() // подключение к серверу
{
    sf::Socket::Status connect_status = m_network->connectToServer(); // подключение к серверу
    if (connect_status != sf::Socket::Done) // если при подключение произошла ошибка
    {
        m_playOnline_Button->setErrorText(SERVER_CONNECTING_ERROR);  // вывод ошибки под кнопкой
    }
    else
    {
        std::wstring str[] = { m_playerName_Input->getWText() };
        m_playerName = m_playerName_Input->getWText(); // получение имени пользователя из поля для ввода текста

        sf::Socket::Status send_status = m_network->sendToServer(ServerCommands::setPlayerName, 1, str); // отправка имени пользователя на сервер
    }
    m_client->setBlocking(false); // отключаем режим блокировки сокета
}
 
void StartMenu::dataProcessing(sf::Packet packet) // обработка данных с сервера
{
    int command;
    packet >> command; // получение комманды из пакета с сервера
    switch (command)
    {
    case (ServerCommands::onPreparationMode): // если пришла комманда на включение расстановки кораблей
    {
        packet >> m_opponentName; // получение имени оппонента из пакета
        m_isPreparationModeEnabled = true; // включение окна расстановки кораблей
        break;
    }
    case (ServerCommands::onWaitingMode):  // если пришла комманда на включение окна ожидания
    {
        m_isWaitingModeEnabled = true; // включение окна ожидания
        break;
    }
    default:
        break;
    }
}

void StartMenu::drawMenu(sf::RenderWindow& window) // отрисовка стартового меню
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case (sf::Event::Closed): // событие закрытия окна
            window.close();
            break;
        case (sf::Event::TextEntered): // событие ввода текста
        {
            m_playerName_Input->editText(event); // редактирования имени
            break;
        }
        case (sf::Event::KeyPressed): // событие нажатия клавиши клавиатуры
        {
            m_playerName_Input->keyPressed(event); // обработка событий нажатия клавиши клавиатуры в поле ввода
            break;
        }
        case (sf::Event::MouseButtonReleased): // событие отпускания кнопки мыши
            if (m_playOnline_Button->getPressedStatus())
            {
                if (m_playerName_Input->getWText().empty()) // если пользователь не ввёл имя
                {
                    m_playerName_Input->setErrorText(EMPTY_NAME_ERROR); // вывод ошибки
                }
                else
                {
                    m_playerName_Input->clearErrorText(); // очистка строки ошибки
                    connectToServer(); // подключение к серверу
                }
                m_playOnline_Button->setPressedStatus(false);
            }
            if (m_waiting_Menu.getButton()->getPressedStatus()) // если кнопка "в главное меню" меню ожидания нажата
            {
                m_isPreparationModeEnabled = false; // выключить мод подготовки
                m_isWaitingModeEnabled = false; // выключить мод ожидания
                m_isSocketDisconnected = true; // включить флаг отключения от сервера
                m_waiting_Menu.getButton()->setPressedStatus(false);
            }
            break;
        default:
            break;
        }

    }

    sf::Packet packet;
    if (m_client->receive(packet) == sf::Socket::Done) // успешно получен пакет информации ?
    {
        dataProcessing(packet); // обработка полученный с сервера информации
    }

    window.draw(m_backgroundSprite); // отрисовка заднего фона
    if (m_isWaitingModeEnabled) // мод ожидания включен ?
    {
        m_waiting_Menu.drawMenu(window); // отрисовка меню ожидания
    }
    else
    {
        m_playOnline_Button->drawButton(window); // отрисовка кнопки "играть онлайн"
        m_playerName_Input->drawLineEdit(window); // отрисовка поля ввода имени
    }
}

// сбросить флаги до дефолтных
void StartMenu::resetFlags()
{
    m_isPreparationModeEnabled = false; // включен ли мод подготовки к битве
    m_isWaitingModeEnabled = false; // включен ли мод ожидания противника
}

// установить флажок отключения от сервера
void StartMenu::setDisconnectedStatus(bool status)
{
    m_isSocketDisconnected = status;
}

// установить флажок открытия меню подготовки
void StartMenu::setPreparationModeStatus(bool status)
{
    m_isPreparationModeEnabled = status;
}

// вернуть имя игрока
std::wstring StartMenu::getPlayerName()
{
    return m_playerName;
}

// вернуть имя противника
std::wstring StartMenu::getOpponentName()
{
    return m_opponentName;
}

// вернуть включен ли режим подготовки
bool StartMenu::getPreparationModeStatus()
{
    return m_isPreparationModeEnabled;
}

// вернуть установлен ли флажок отключения от сервера
bool StartMenu::getDisconnectedStatus()
{
    return m_isSocketDisconnected;
}