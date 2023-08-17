//
// Created by Ilya on 17.08.2023.
//

#ifndef SFML_SORTING_PLAYER_H
#define SFML_SORTING_PLAYER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gtime.h"
#include "Ball.h"

class Ball;

using namespace std;
class Player
{
private:

public:
    int id;
    float speed = 2;
    sf::Vector2f pos;
    sf::Vector2f size;

    Player(int id, float speed, sf::Vector2u windowSize);

    void Move(sf::Vector2i dir, sf::Vector2u winSize);

    [[nodiscard]] sf::Vector2f HalfSize() const;

    [[nodiscard]] sf::RectangleShape GetShape() const;

    bool CheckBallCollision(Ball& ball) const;
};

#endif //SFML_SORTING_PLAYER_H
