#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

class Player {
public:
	std::string playerName = "playerName";
	int playerHealth = 100;
	int playerDamage = 0;
	float playerAttackSpeed = 0.0f;
	// index gets multiplied by 64 to find the position of single sprite in the spritesheet
	// index is the count of sprites in the sheet, count starts at 0
	int xIndex = 0;
	int yIndex = 0;
	// create sprite object to turn image into a sprite which can be moved, rotated, scaled etc.
	sf::Sprite sprite;
private:
	// create texture object to hold .png image
	sf::Texture texture;
public:
	void Initialize();
	void Load();
	void Update();
	void Draw(sf::RenderWindow& window);
private:

};