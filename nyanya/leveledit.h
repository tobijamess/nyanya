#ifndef LEVELEDIT_H
#define LEVELEDIT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "map.h"
#include <vector>

class Game;

class LevelEdit {
public:
	int tileOptionIndex;
	sf::Texture texture;
	// vector of tiles themselves
	std::vector<sf::Sprite> tiles;
	// vector of different tile images to choose from
	std::vector<sf::Sprite> tileOptions;
	void CreateTile(const sf::Vector2f& position);
	void RemoveTile(const sf::Vector2f& position);
	void Initialize(Game& game);
	void Load();
	void Update(sf::RenderWindow& window, Game& game);
	void Draw(sf::RenderWindow& window);
private:
};
#endif
