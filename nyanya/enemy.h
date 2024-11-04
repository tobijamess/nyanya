#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

// see player.h for commenting
class Enemy {
public:
	std::string enemyName = "enemyName";
	int enemyHealth = 100;
	int enemyDamage = 0;
	float enemyAttackSpeed = 0.0f;

	int xIndex = 0;
	int yIndex = 0;

	sf::Sprite sprite;
private:
	sf::Texture texture;
public:
	void Initialize();
	void Load();
	void Update();
	void Draw(sf::RenderWindow& window);
private:
};