#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <list>

#include "network.h"

class Menu
{
public:
	Menu(std::string backgroundimage, sf::TcpSocket* socket = nullptr);
	~Menu();

	void drawMenu(sf::RenderWindow& window);
	void setNewSocket(sf::TcpSocket* socket);

protected:
	sf::TcpSocket* m_client;
	NetWork* m_network;

	sf::Image m_backgroundImage;
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	bool m_isSocketDisconnected = false;
};

Menu::Menu(std::string backgroundimage, sf::TcpSocket* socket)
{
	setNewSocket(socket);
	m_backgroundImage.loadFromFile(backgroundimage);
	m_backgroundTexture.loadFromImage(m_backgroundImage);
	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, m_backgroundImage.getSize().x, m_backgroundImage.getSize().y));
	m_backgroundSprite.setPosition(0, 0);
}

Menu::~Menu()
{
	delete m_network;
}

void Menu::setNewSocket(sf::TcpSocket* socket)
{
	m_client = socket;
	delete m_network;
	m_network = new NetWork(socket);
	m_isSocketDisconnected = false;
}