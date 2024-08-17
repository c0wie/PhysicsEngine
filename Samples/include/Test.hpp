#pragma once

#include "Shape.hpp"
#include <imgui.h>
#include "PhysicsWorld.hpp"
#include <chrono>

struct Stoper {
    // Use chrono to represent time points
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool running = false;

    // Start the timer
    void start() {
        std::cout << "Stoper starts\n";
        startTime = std::chrono::high_resolution_clock::now();
        running = true;
    }

    // Stop the timer
    void stop() {
        if (running) {
            std::cout << "Stoper stops\n";
            endTime = std::chrono::high_resolution_clock::now();
            running = false;
        } else {
            std::cout << "Stoper is not running. Please start it first.\n";
        }
    }

    // Print the elapsed time
    void printTime() const {
        if (!running) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            std::cout << "Elapsed time: " << elapsed.count() << " milliseconds\n";
        } else {
            std::cout << "Stoper is still running. Please stop it first.\n";
        }
    }

    // Reset the timer
    void reset() {
        startTime = std::chrono::time_point<std::chrono::high_resolution_clock>();
        endTime = std::chrono::time_point<std::chrono::high_resolution_clock>();
        running = false;
    }
};
namespace test
{
    class Test
    {
    public:
        Test() = default;
        virtual ~Test() = default;
    public:
        virtual void OnUpdate(float deltaTime, sf::Vector2i mousePos) = 0;
        virtual void OnRender(sf::RenderWindow &window) = 0;
        virtual void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO& io) = 0;
    protected:
        void AddCircle(const sf::Color color, pe2d::RigidObject &object);
        void AddCircle(size_t ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isStatic,
                        float mass, pe2d::Vector2 velocity,pe2d::Vector2 gravity, float staticFriction,
                        float dynamicFriction, float restitution);
        void AddBox(const sf::Color color, pe2d::RigidObject &object);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isStatic,
                    float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity, float staticFriction,
                    float dynamicFriction, float restitution);
        
        void Draw(sf::RenderWindow &window) const;
    protected:
        std::vector<Shape> m_Shapes;
        pe2d::PhysicsWorld m_World;
    };
    void DrawCircle(sf::RenderWindow &window, const Shape &shape);
    void DrawBox(sf::RenderWindow &window, const Shape &shape);
}