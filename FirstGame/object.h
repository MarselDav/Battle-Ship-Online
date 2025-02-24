#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Object
{
public:
	Object(float x, float y, float width, float height, std::string imagefile);

	sf::Sprite* getSprite();
	sf::Vector2f getStartPos();
	sf::Vector2f getPos();
	sf::Vector2f getSize();

private:
	sf::Vector2f current_pos;
	sf::Vector2f start_pos;

	sf::Image image;
	sf::Texture texture;

protected:
	sf::Sprite sprite;
};

Object::Object(float x, float y, float width, float height, std::string imagefile)
{
	image.loadFromFile(imagefile);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, image.getSize().x, image.getSize().y));
	sprite.setScale(width / image.getSize().x, height / image.getSize().y);

	sprite.setPosition(x, y);
	start_pos = sf::Vector2f(x, y);
}

sf::Sprite* Object::getSprite()
{
	return &sprite;
}

sf::Vector2f Object::getStartPos()
{
	return start_pos;
}

sf::Vector2f Object::getPos()
{
	return sprite.getPosition();
}

sf::Vector2f Object::getSize()
{
	return sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
}