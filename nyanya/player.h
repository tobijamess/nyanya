#include <iostream>
#include <string>

class PlayerClass {
public:

	// intialize player by setting to true using gameStart(); when its called in main loop
	bool playerInit;

	// unset player variables and values
	std::string playerName = "playerName";
	int playerHealth = 100;
	int playerDamage = 0;
	float playerAttackSpeed = 0.0f;

	// values for moving players position and speed
	float playerX = 0.0f, playerY = 0.0f;
	float playerSpeed = 0.0f;
	
	float playerMove;

private:


};

// func to initalize player using playerInit
int initPlayer();
// func to handle player input
void playerInput();