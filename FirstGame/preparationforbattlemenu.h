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

    void getStartMenuStatus(bool status); // установить статус  перехода в стартовое меню
    void setBattleModeStatus(bool status); // установить статус перехода в меню битвы

    bool getBattleModeStatus(); // вернуть статус перехода в меню битвы
    bool getStartMenuStatus(); // вернуть статус перехода в стартовое меню

    std::vector<Ship*> getShipsOnTheTableVector(); // вернуть вектор кораблей находящихся на столе

    sf::Vector2f getCellularFieldPosition(); // вернуть позицию поля


private:
    bool checkDuplicate(std::vector<Ship*>& shipsVector, Ship* ship); // проверить повторение корабля в векторе
    void mouseButtonPressedEvent(const sf::Event& event, sf::RenderWindow& window); // обработка событий нажатия кнопки мыши
    void mouseButtonReleasedEvent(const sf::Event& event, CellularField& cellularfield); // обработка событий отпускания кнопки мыши
    void mouseMoveEvent(CellularField& cellularfield, sf::RenderWindow& window); // обработка событий движения мыши
    void dataProcessing(sf::Packet packet); // обработка данных с сервера

    void startBattleMode(); // начать режим битвы

    bool m_isBattleModeEnabled = false; // включен ли режим битвы
    bool m_isWaitingModeEnabled = false; // включен ли режим ожидания

    InfoMenu m_waiting_Menu = InfoMenu(WAITING_OPPONENT_PREPARATION_STRING); // меню ожидания

    PreparationCellularField *m_preparationСellularField; // поле расстановки кораблей
    Button *m_goToStartMenu_Button; // кнопка вернуться в главное меню
    Button *m_goToBattleMenu_Button; // кнопка битва

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


    std::vector<Ship*> m_shipsOnTheTableVector; // вектор кораблей на столе

    Ship* m_currentMovableShip = nullptr; // текущий передвигаемый корабль

    std::wstring m_opponentName; // имя противника
};

PreparationForBattleMenu::PreparationForBattleMenu(sf::TcpSocket* socket): Menu(BG_PREPARATION_IMAGE, socket)
{
    m_preparationСellularField = new PreparationCellularField(CELLULARFIELD_X, CELLULARFIELD_Y);
    m_goToStartMenu_Button = new  Button(GO_TO_START_MENU_X, GO_TO_START_MENU_Y, GO_TO_START_MENU_WIDTH, GO_TO_START_MENU_HEIGHT, TO_START_MENU_STRING);
    m_goToBattleMenu_Button = new Button(GO_TO_BATTLE_MENU_X, GO_TO_BATTLE_MENU_Y, GO_TO_BATTLE_MENU_WIDTH, GO_TO_BATTLE_MENU_HEIGHT, BATTLE_TITLE);
}

PreparationForBattleMenu::~PreparationForBattleMenu()
{
    delete m_preparationСellularField;
    delete m_goToStartMenu_Button;
    delete m_goToBattleMenu_Button;
}

void PreparationForBattleMenu::startBattleMode() // начать режим битвы
{
    if (m_shipsOnTheTableVector.size() == SHIPS_CNT) // все корабли расставлены?
    {
        int shipsParams[SHIPS_PARAMS_CNT * SHIPS_CNT];
        int i = 0;
        for (int j = 0; j < SHIPS_CNT; j++) // проход по всем кораблям
        {
            // параметры корабля
            int c_x = ((m_shipsOnTheTableVector[j])->getPos().x - m_preparationСellularField->getPosition().x) / CELL_SIZE;
            int c_y = ((m_shipsOnTheTableVector[j])->getPos().y - m_preparationСellularField->getPosition().y) / CELL_SIZE;
            int c_w =(m_shipsOnTheTableVector[j])->getSize().x / CELL_SIZE;
            int c_h = (m_shipsOnTheTableVector[j])->getSize().y / CELL_SIZE;

            // запись параметров в массив
            shipsParams[i] = c_x;
            shipsParams[i + 1] = c_y;
            shipsParams[i + 2] = c_w;
            shipsParams[i + 3] = c_h;
            i += SHIPS_PARAMS_CNT;
        }
        m_network->sendToServer(ServerCommands::readyToBattle, SHIPS_PARAMS_CNT * SHIPS_CNT, shipsParams); // отправка команды готовности к битве и массив кораблей
    }
    else
        m_goToBattleMenu_Button->setErrorText(NOT_ALL_SHIPS_ON_THE_TABLE_ERROR); // вывод ошибки, что не все корабли расставлены
}


bool PreparationForBattleMenu::checkDuplicate(std::vector<Ship*>& shipsVector, Ship* ship) // проверить повторение корабля в векторе
{
    bool isDuplicate = false; // нет повторов
    if (shipsVector.size() != 0) // если вектор не пустой
    {
        int i = 0;
        while (!isDuplicate && (i < shipsVector.size())) // проход по всем корабля
        {
            if (shipsVector[i] == ship) // если корабли одинаковые
                isDuplicate = true; // есть повтор
            i++;
        }
    }
    return isDuplicate;
}

void PreparationForBattleMenu::dataProcessing(sf::Packet packet) // обработка данных с сервера
{
    int command;
    packet >> command; // получение команды из пакета
    switch (command)
    {
    case (ServerCommands::onBattleMode): // команда начала битвы
    {
        packet >> m_opponentName; // получение имени противника из пакета
        std::wcout << L"Имя противника: " << m_opponentName << std::endl;
        m_isBattleModeEnabled = true; // включен режим битвы
        break;
    }
    case (ServerCommands::onWaitingMode): // команда ожидания
    {
        m_isWaitingModeEnabled = true; // включен режим ожидания
        break;
    }
    default:
        break;
    }
}

void PreparationForBattleMenu::mouseButtonPressedEvent(const sf::Event& event, sf::RenderWindow& window) // обработка событий нажатия кнопки мыши
{
    if (event.key.code == sf::Mouse::Left) // если нажата левая кнопка
    {
        // проверка на нажатия по кораблям
        for (int i = 0; i < m_shipsvector.size(); i++)
        {
            sf::FloatRect shipRect = (m_shipsvector[i])->getSprite()->getGlobalBounds(); // прямоугольник корабля
            sf::FloatRect mouseRect = sf::FloatRect(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 1, 1); // прямоугольник курсора
            if (shipRect.intersects(mouseRect)) // курсор мыши наведене на корабль?
            {
                m_currentMovableShip = m_shipsvector[i]; // текущий передвигаемый корабль

                int j = 0;
                bool isDeleted = false; // флаг не удаление
                while (j < m_shipsOnTheTableVector.size() && !isDeleted) // проход по всем кораблям
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

void PreparationForBattleMenu::mouseButtonReleasedEvent(const sf::Event& event, CellularField& cellularfield)  // обработка событий отпускание кнопки мыши
{ 
    if (event.key.code == sf::Mouse::Left) // если отпущена левая кнопка
    {
        if (m_currentMovableShip != nullptr) // если текущий передвигаемый корабль существует
        {
            float x, y;
            if (m_preparationСellularField->getCanPutStatus()) // если корабль можно поставить
            {
                x = m_preparationСellularField->getRectPos().x; // новая позиция x корабля
                y = m_preparationСellularField->getRectPos().y; // новая позиция y корабля
                if (!checkDuplicate(m_shipsOnTheTableVector, m_currentMovableShip)) // если там не стоит корабля
                    m_shipsOnTheTableVector.push_back(m_currentMovableShip); // добавление корабля в вектор кораблей на столе
            }
            else
            {
                x = m_currentMovableShip->getStartPos().x; // новая позиция x корабля
                y = m_currentMovableShip->getStartPos().y; // новая позиция y корабля
            }


            m_currentMovableShip->getSprite()->setPosition(x, y); // изменение позиции корабля, он ставится на стол
            sf::Sprite sprite = *m_currentMovableShip->getSprite();
            m_preparationСellularField->setIsDrawRectStatus(false);
            m_currentMovableShip = nullptr;
        }

        if (m_goToBattleMenu_Button->getPressedStatus()) // если нажата кнопка перехода в меню битвы
        {
            startBattleMode(); // начать режим битвы
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
        m_preparationСellularField->checkCollision(shipRect.left, shipRect.top,
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
        case (sf::Event::Closed): // событие закрытия окна
            window.close();
            break;
        case (sf::Event::MouseButtonPressed):
        {
            mouseButtonPressedEvent(event, window);
            break;
        }
        case (sf::Event::MouseButtonReleased):
        {
            mouseButtonReleasedEvent(event, *m_preparationСellularField);
            break;
        }
        case (sf::Event::MouseMoved):
        {
            mouseMoveEvent(*m_preparationСellularField, window);
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
        m_preparationСellularField->drawRectangle(&window, m_shipsOnTheTableVector);
        m_preparationСellularField->drawField(&window);
        for (int i = 0; i < m_shipsvector.size(); i++)
        {
            if (m_shipsvector[i] != nullptr)
                window.draw(*(m_shipsvector[i]->getSprite()));
            else
                std::cout << "Схуяли нулевой" << std::endl;
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
    return m_preparationСellularField->getPosition();
}

bool PreparationForBattleMenu::getStartMenuStatus()
{
    if (m_goToStartMenu_Button->getPressedStatus() || m_waiting_Menu.getButton()->getPressedStatus())
        return true;
    return false;
}