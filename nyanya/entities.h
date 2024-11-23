#ifndef ENTITIES_H
#define ENTITIES_H
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
class Entity {
protected:
    sf::Texture texture;        // Texture to assign to the sprite
    sf::Sprite sprite;          // Sprite for the entity
    sf::CircleShape hitbox;     // Hitbox for collision
    sf::Vector2f size;          // Size of the entity
    float speed;                // Speed for movement

public:
    Entity() : speed(0.0f) {}   // Default constructor

    // Initialize the entity with default settings
    virtual void Initialize(const sf::Vector2f& entitySize, float entitySpeed);

    // Set position of the entity
    void SetPosition(const sf::Vector2f& position);

    // Get position of the entity
    sf::Vector2f GetPosition() const;

    // Get texture object
    const sf::Texture GetTexture() const;

    // Get sprite object
    const sf::Sprite GetSprite() const;

    // Get hitbox for collision
    const sf::CircleShape& GetHitbox() const;

    // Move the entity by an offset
    virtual void Move(const sf::Vector2f& offset);

    // Draw entity to window
    virtual void Draw(sf::RenderWindow& window) const;
};
#endif