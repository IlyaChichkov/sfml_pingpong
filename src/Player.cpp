//
// Created by Ilya on 17.08.2023.
//

#include "Player.h"


Player::Player(int id, float speed, sf::Vector2u windowSize)
{
    this->id = id;
    this->speed = speed;
    size = sf::Vector2f(20, 90);

    switch (id) {
        case 1:
            pos = sf::Vector2f(size.x * 1.5f, (float)windowSize.x / 2);
            break;
        case 2:
            pos = sf::Vector2f((float)windowSize.x - size.x * 1.5f, (float)windowSize.x / 2);
            break;
        default:
            throw std::runtime_error("Wrong player ID");
    }
}

void Player::Move(sf::Vector2i dir, sf::Vector2u winSize)
{
    float dt = ProgramTime::deltaTime();
    sf::Vector2f change((float)dir.x * dt * speed, (float)dir.y * dt * speed);
    // cout << "change y: " << change.y  << endl;
    sf::Vector2f newPos = pos + change;
    if(newPos.y < size.y / 2) { return; }
    if(newPos.y > (float)winSize.y - size.y / 2) { return; }
    pos = newPos;
}

[[nodiscard]] sf::Vector2f Player::HalfSize() const {
    return {size.x / 2, size.y / 2};
}

[[nodiscard]] sf::RectangleShape Player::GetShape() const
{
    sf::RectangleShape block(size);
    block.setPosition(pos - HalfSize());
    return block;
}

bool Player::CheckBallCollision(Ball& ball) const
{
    sf::Vector2f ballPos = ball.GetPosition();
    float radius = ball.GetRadius();
    if((pos.x + HalfSize().x > ballPos.x - radius) && (pos.x - HalfSize().x < ballPos.x + radius)) {
        if((pos.y + HalfSize().y > ballPos.y - radius) && (pos.y - HalfSize().y < ballPos.y + radius))
        {
            // cout << ">> COLLISION player " << id << " is true" << endl;
            ball.Bounce();
            return true;
        }
    }
    return false;
}

