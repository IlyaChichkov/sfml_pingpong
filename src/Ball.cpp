//
// Created by Ilya on 17.08.2023.
//

#include "Ball.h"

Ball::Ball(float ballRadius, float speed)
{
    this->ballRadius = ballRadius;
    this->speed = speed;
}

void Ball::Spawn(sf::Vector2f pos, sf::Vector2f moveDir)
{
    if(this->isAlive) return;
    this->pos = pos;
    this->moveDir = moveDir;
    this->isAlive = true;
    std::thread ballMove([this](){
        this->Move();
    });
    ballMove.detach();
}

void Ball::Move()
{
    while(isAlive){
        float speed_mul = 1;
        if(p1->CheckBallCollision(*this) ||
           p2->CheckBallCollision(*this)){
            speed_mul = 1.2f;
        }

        float dt = ProgramTime::deltaTime();
        //cout << "dt: " << dt << endl;
        //cout << "speed: " << speed << endl;
        //cout << "(float)moveDir.x: " << (float)moveDir.x << endl;
        sf::Vector2f change((float)moveDir.x * dt * speed * speed_mul, (float)moveDir.y * dt * speed * speed_mul);
        //cout << "x: " << change.x << "; y: " << change.y << endl;
        sf::Vector2f newPos = pos + change;
        cout << "newPos x: " << newPos.x << "; y: " << newPos.y << endl;

        if(newPos.y - ballRadius < 0)   { moveDir.y *= -1;
            continue; }
        if(newPos.y + ballRadius > (float)windowSize.y) { moveDir.y *= -1;
            continue;  }

        if(newPos.x - ballRadius < 0)   { isAlive = false; idScore = 2; }
        if(newPos.x + ballRadius > (float)windowSize.x) { isAlive = false; idScore = 1; }

        pos = newPos;
    }
}

sf::Vector2f Ball::GetPosition()
{
    return pos;
}

float Ball::GetRadius()
{
    return ballRadius;
}

int Ball::GetScoreID()
{
    int temp = idScore;
    idScore = 0;
    return temp;
}

void Ball::Destroy()
{
    isAlive = false;
}

[[nodiscard]] sf::CircleShape Ball::GetShape() const
{
    if(!isAlive){
        sf::CircleShape circle(ballRadius);
        circle.setFillColor(sf::Color::Red);
        return circle;
    }
    sf::CircleShape circle(ballRadius);
    circle.setPosition(pos - sf::Vector2f(ballRadius, ballRadius));
    return circle;
}

void Ball::Bounce()
{
    moveDir.x *= -1;
}

void Ball::SetPlayers(Player *pl1, Player *pl2) {
    p1 = pl1;
    p2 = pl2;
}
