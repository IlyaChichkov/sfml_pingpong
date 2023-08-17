#include <iostream>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <cstring>
#include <chrono>
#include <sstream>
#include <thread>
#include "src/gtime.h"
#include "src/Ball.h"
#include "src/Player.h"

using namespace std;

template<typename T>
T getRandom(T fMin, T fMax)
{
    float f = (float)rand() / RAND_MAX;
    cout << "getRandom: " << (float)f << endl;
    T r = fMin + f * (fMax - fMin);
    return r;
}


class PingPong
{

    bool anyPressed = false;

    std::unordered_map<sf::Keyboard::Key, bool> pressedKeys;

    static sf::Vector2f Norm(sf::Vector2f v)
    {
        float length = sqrt((v.x * v.x) + (v.y * v.y));
        return {v.x / length, v.y / length};
    }

private:
    void CheckBallScore()
    {
        int scoreId = ball->GetScoreID();
        if(scoreId != 0)
        {
            SpawnBall();
        }
    }

    void DrawShapes()
    {
        sf::RenderWindow& renderWindow = *window;
        renderWindow.draw(p1->GetShape());
        renderWindow.draw(p2->GetShape());
        sf::CircleShape p1c(1);
        p1c.setPosition(p1->pos);
        p1c.setFillColor(sf::Color::Green);
        renderWindow.draw(p1c);
        renderWindow.draw(ball->GetShape());
    }

public:
    Ball* ball;

    sf::RenderWindow* window{};
    Player* p1;
    Player* p2;

    PingPong(Player* p1, Player* p2, sf::RenderWindow& renderWindow)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->window = &renderWindow;

        this->ball = new Ball(15, 3);
        this->ball->windowSize = window->getSize();
        this->ball->SetPlayers(p1, p2);
    }

    void SetWindow(sf::RenderWindow& renderWindow)
    {
        this->window = &renderWindow;
    }

    void EventHandler(sf::Event& event)
    {
        // Check for key press events
        if (event.type == sf::Event::KeyPressed)
        {
            pressedKeys[event.key.code] = true;
        }
        // Check for key release events
        if (event.type == sf::Event::KeyReleased)
        {
            pressedKeys[event.key.code] = false;

            if (event.key.code == sf::Keyboard::Space)
            {
                SpawnBall();
            }
        }
        if(!anyPressed){
            std::thread input([this](){
                InputHandler();
            });
            input.detach();
        }
    }

    void InputHandler()
    {
        anyPressed = true;
        while (anyPressed)
        {
            anyPressed = false;

            sf::Vector2i p1Dir(0, 0);
            sf::Vector2i p2Dir(0, 0);
            if (pressedKeys[sf::Keyboard::Left])
            {
                p2Dir = sf::Vector2i(0, -1);
                anyPressed = true;
            }
            if (pressedKeys[sf::Keyboard::Right])
            {
                p2Dir = sf::Vector2i(0, 1);
                anyPressed = true;
            }
            if (pressedKeys[sf::Keyboard::A])
            {
                p1Dir = sf::Vector2i(0, -1);
                anyPressed = true;
            }
            if (pressedKeys[sf::Keyboard::D])
            {
                p1Dir = sf::Vector2i(0, 1);
                anyPressed = true;
            }
            p1->Move(p1Dir, this->window->getSize());
            p2->Move(p2Dir, this->window->getSize());
        }
    }

    void Update()
    {
        CheckBallScore();
        DrawShapes();
    }

    void SpawnBall()
    {
        sf::Vector2u windowSize = window->getSize();
        sf::Vector2f screenCenter((float)windowSize.x / 2, (float)windowSize.y / 2);

        int xDirection = getRandom<float>(0, 100) > 50 ? 1: -1;
        sf::Vector2f dir = Norm(sf::Vector2f((float)xDirection, getRandom<float>(-1, 1)));
        ball->Spawn(screenCenter, dir);
    }

    ~PingPong()
    {
        delete(this->ball);
    }
};

void SortingCoreInitialize(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "test");

    Player p1(1, 0.010, window.getSize());
    Player p2(2, 0.010, window.getSize());
    PingPong pingpong(&p1, &p2, window);

    window.setFramerateLimit(60);

    // Initialize ImGui
    ImGui::SFML::Init(window);

    // Main loop
    while (window.isOpen())
    {
        ProgramTime::RestartClock();
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else
            {
                pingpong.EventHandler(event);
                if (event.type == sf::Event::Resized) {
                    // Handle window resize
                    sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
                    window.setView(sf::View(visibleArea));
                }
            }
        }


        ImGui::SFML::Update(window, ProgramTime::elapsedTime);

        // Clear the windows
        window.clear();

        pingpong.Update();

        ImGui::SFML::Render(window);

        window.display();
    }

    // Shutdown ImGui
    ImGui::SFML::Shutdown();
}

int main()
{
    srand(time(NULL));
    SortingCoreInitialize();
    return 0;
}