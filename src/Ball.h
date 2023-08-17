//
// Created by Ilya on 17.08.2023.
//

#ifndef SFML_SORTING_BALL_H
#define SFML_SORTING_BALL_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include "gtime.h"
#include "Player.h"

class Player;

using namespace std;
class Ball
{
    float ballRadius = 15;

    Player* p1;
    Player* p2;

    sf::Vector2f pos;
    sf::Vector2f moveDir;
    float speed;
    bool isAlive = false;
    int idScore = 0;

public:
    sf::Vector2u windowSize;

    Ball(float ballRadius, float speed);

    void Spawn(sf::Vector2f pos, sf::Vector2f moveDir);

    void Move();

    void SetPlayers(Player* p1, Player* p2);

    sf::Vector2f GetPosition();

    float GetRadius();

    int GetScoreID();

    void Destroy();

    [[nodiscard]] sf::CircleShape GetShape() const;

    void Bounce();
};

#endif //SFML_SORTING_BALL_H
