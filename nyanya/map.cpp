#include "map.h"

//GameTile::GameTile(std::string textureName, float x, float y, bool passable, bool exit) {
//
//	if (!setUpSprite(textureName)) {
//		return;
//	}
//
//	pos = sf::Vector2f(x, y);
//	sprite.setPosition(pos);
//
//	isPassable = passable;
//	isExit = exit;
//}
//
//bool GameTile::setUpSprite(std::string textureName) {
//	if (texture.loadFromFile(textureName)) {
//		std::cout << "map image loaded\n";
//	}
//	else {
//		std::cout << "map image didnt load\n";
//	}
//	sprite.setTexture(texture);
//	sprite.setTextureRect(sf::IntRect(0, 0, 500, 1000));
//	return true;
//}