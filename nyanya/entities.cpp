#include "entities.h"

void Entity::Initialize(const sf::Vector2f& entitySize, float entitySpeed) {
    size = entitySize;
    speed = entitySpeed;

    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1);
    hitbox.setRadius(entitySize.x / 2.0f);
    hitbox.setOrigin(entitySize.x / 2.0f, entitySize.y / 2.0f);
}

void Entity::SetPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
    hitbox.setPosition(position);
}

sf::Vector2f Entity::GetPosition() const {
    return sprite.getPosition();
}

const sf::CircleShape& Entity::GetHitbox() const {
    return hitbox;
}

const sf::Texture Entity::GetTexture() const {
    return texture;
}

const sf::Sprite Entity::GetSprite() const {
    return sprite;
}

void Entity::Move(const sf::Vector2f& offset) {
    sprite.move(offset);
    hitbox.move(offset);
}

void Entity::Draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    window.draw(hitbox);
}
