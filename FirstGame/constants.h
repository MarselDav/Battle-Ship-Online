#pragma once

#include <vector>

// размеры окна
const int WIDTH = 1200;
const int HEIGHT = 700;

const std::string fontName = "Srbija Sans.otf"; // шрифт текста

const unsigned short PORT = 5000;
const std::string BG_START_IMAGE = "images/bg_ships.png"; // задний фон для стартового меню
const std::string BG_WAITING_IMAGE = "images/bgBattleMenu.png"; // задний фон для меню ожидания
const std::string BG_PREPARATION_IMAGE = "images/bg.png"; // задний фон для меню подготовки

// Ошибки start menu
const std::wstring EMPTY_NAME_ERROR = L"Введите своё имя";
const std::wstring SERVER_CONNECTING_ERROR = L"Ошибка подключения к серверу";
const std::wstring SEND_PACKET_ERROR = L"Ошибка отправки пакета";

// Сообщения start menu
const std::wstring CONNECTION_SUCCESSFUL = L"Подключение прошло успешно";
const std::wstring SEND_PACKET_SUCCESSFUL = L"Пакет успешно отправлен";

const std::wstring  PLAY_ONLINE_BUTTON_STRING = L"Играть онлайн";
const std::wstring  TO_START_MENU_STRING = L"В главное меню";

const std::wstring WAITING_OPPONENT_CONNECT_STRING = L"Ожидание подключения противника....";
const std::wstring WAITING_OPPONENT_PREPARATION_STRING = L"Ожидание готовности противника....";

const std::wstring WIN_STRING = L"Вы выиграли!";
const std::wstring LOSE_STRING = L"Вы проиграли...";

const enum ServerCommands // команды сервера
{
    setPlayerName = 0,
    onPreparationMode = 1,
    onWaitingMode = 2,
    readyToBattle = 3,
    onBattleMode = 4,
    myTurn = 5,
    opponentTurn = 6,
    shot = 7,
    myHit = 8,
    myMiss = 9,
    opponentHit = 10,
    opponentMiss = 11,
    destroyed = 12,
    win = 13,
    lose = 14,
};

// cellularfield
const int FRAME_THICKNESS = 5;
const int CELL_SIZE = 45;
const int CELLS_CNT = 10;
const int SHIPS_CNT = 10;

const std::vector<std::string> lettersList = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "G" };

const sf::Color LINE_COLOR(93, 182, 213);

const int CELLULARFIELD_X = 120;
const int CELLULARFIELD_Y = 100;

const int ROTATE_ANGLE = 90;

const int SHIPS_PARAMS_CNT = 4;

// размеры кнопки "играть онлайн"
const int PLAY_BUTTON_WIDTH = 400;
const int PLAY_BUTTON_HEIGHT = 100;
//

// размеры поля "ввод имени"
const int NAME_INPUT_WIDTH = 400;
const int NAME_INPUT_HEIGHT = 100;
//

// координаты кнопки "В главное меню"
const int GOBACK_MENU_BUTTON_X = 50;
const int GOBACK_MENU_BUTTON_Y = 50;

// размеры кнопки "В главное меню"
const int GOBACK_BUTTON_WIDTH = 300;
const int GOBACK_BUTTON_HEIGHT = 50;
//

// смещения кнопки "играть онлайн" относительно центра
const int PLAY_BUTTON_OFFSET = 100;
// смещения поля "ввод имени" относительно центра
const int NAMEINPUT_OFFSET = -100;

const int MAX_LINE_EDIT_SIMBOLS_CNT = 15;

// позиция кнопки вернуться в главное меню
const int GO_TO_START_MENU_X = CELLULARFIELD_X;
const int GO_TO_START_MENU_Y = 600;
// размеры кнопки вернуться в главное меню
const int GO_TO_START_MENU_WIDTH = CELL_SIZE * 5;
const int GO_TO_START_MENU_HEIGHT = 70;

// позиция кнопки битва
const int GO_TO_BATTLE_MENU_X = CELLULARFIELD_X + 6 * CELL_SIZE;
const int GO_TO_BATTLE_MENU_Y = 600;
// размеры кнопки битва
const int GO_TO_BATTLE_MENU_WIDTH = CELL_SIZE * 3;
const int GO_TO_BATTLE_MENU_HEIGHT = 70;

sf::Font font;


const int OUTLINE_THICKNESS = 1;
const int OUTLINE_HOVER_THICKNESS = 2;
const int OUTLINE_SELECT_THICKNESS = 3;

const int DEFAULT_TITLE_CHARACTER_SIZE = 30;
const int DEFAULT_ERROR_CHARACTER_SIZE = 30;

const sf::Color DEFAULT_WIDGET_COLOR(220, 220, 220);
const sf::Color DEFAULT_TITLE_COLOR(96, 115, 162);
const sf::Color DEFAULT_BORDER_COLOR(30, 30, 30);
const sf::Color DEFAULT_HOVER_BORDER_COLOR = sf::Color::Black;
const sf::Color ERROR_COLOR = sf::Color::Red;

const sf::Color LINE_EDIT_BG(220, 220, 220, 150);


const std::string CELLS1_SHIP = "images/ships/1CellsShip.png";
const std::string CELLS2_SHIP = "images/ships/2CellsShip.png";
const std::string CELLS3_SHIP = "images/ships/3CellsShip.png";
const std::string CELLS4_SHIP = "images/ships/4CellsShip.png";

const char CELL_SHIP_FORMAT_STRING[] = "images/ships/%dCellsShip.png";

const std::wstring BATTLE_TITLE = L"БИТВА";

const std::wstring NOT_ALL_SHIPS_ON_THE_TABLE_ERROR = L"Не все корабли расставлены";


const unsigned ARROW_X = 525;
const unsigned ARROW_Y = 250;
const float ARROW_SCALE_X = 0.2;
const float ARROW_SCALE_Y = 0.2;
const std::string GREEN_ARROW_IMAGE = "images/arrow/green_arrow.png";
const std::string RED_ARROW_IMAGE = "images/arrow/red_arrow.png";

const std::string IP_ADRESS_FILE = "ipadress.txt";



// battlemenu
const int PLAYER_CELLULARFIELD_X = 60;
const int PLAYER_CELLULARFIELD_Y = 100;

const int PLAYER_NAME_X = PLAYER_CELLULARFIELD_X;
const int PLAYER_NAME_Y = PLAYER_CELLULARFIELD_Y - 80;

const int OPPONENT_CELLULARFIELD_X = 700;
const int OPPONENT_CELLULARFIELD_Y = 100;

const int OPPONENT_NAME_X = OPPONENT_CELLULARFIELD_X;
const int OPPONENT_NAME_Y = PLAYER_CELLULARFIELD_Y - 80;

const std::string BG_BATTLE_IMAGE = "images/bgBattleMenu.png";
const std::string HIT_IMAGE = "images/hit.png";
const std::string MISS_IMAGE = "images/miss.png";