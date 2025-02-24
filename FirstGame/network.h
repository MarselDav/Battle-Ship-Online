#pragma once

#include <SFML/Network.hpp>
#include <iostream>

#include "constants.h"


class NetWork
{
public:
	NetWork(sf::TcpSocket* socket);

    template <typename T>
    sf::Socket::Status sendToServer(int command, int size, const T* dataArray);
    sf::Socket::Status connectToServer();


private:
	sf::TcpSocket* client;
};

NetWork::NetWork(sf::TcpSocket* socket)
{
	client = socket;
}

sf::Socket::Status NetWork::connectToServer()
{
    sf::IpAddress ipAdress;
    std::fstream ipAdressFile(IP_ADRESS_FILE);
    if (ipAdressFile)
    {
        ipAdressFile >> ipAdress;
    }
    std::cout << ipAdress << std::endl;
    ipAdressFile.close();


    sf::Socket::Status status = client->connect(ipAdress, PORT);
    
    if (status != sf::Socket::Done)
    {
        std::wcout << SERVER_CONNECTING_ERROR << std::endl;
    }
    else
    {
        std::wcout << CONNECTION_SUCCESSFUL << std::endl;
    }
    client->setBlocking(false);

    return status;
}

template <typename T>
sf::Socket::Status NetWork::sendToServer(int command, int size, const T* dataArray)
{
    sf::Packet packet;
    packet << command;
    for (int i = 0; i < size; i++)
    {
        //std::cout << "arg " << i << ": " << dataArray[i] << std::endl;
        packet << dataArray[i];
    }
    sf::Socket::Status status = client->send(packet);
    if (status != sf::Socket::Done)
    {
        std::wcout << SEND_PACKET_ERROR << std::endl;
    }
    else
    {
        std::wcout << SEND_PACKET_SUCCESSFUL << std::endl;
    }

    return status;
}