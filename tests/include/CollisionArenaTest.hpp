#include "Test.hpp"

namespace test
{
    struct Color
    {
        int red, green, blue;
    };
    enum ID
    {
        BOX, 
        CIRCLE,
    };
    class CollisionArenaTest final : public Test
    {
    public:
        CollisionArenaTest();
        ~CollisionArenaTest() = default;
    public:
        void OnUpdate(float deltaTime, sf::Vector2i mousePos) override;
        void OnRender(sf::RenderWindow &window) override;
        void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io) override;
    private:
        void CollisionObjectInput();
        void RigidObjectInput();
        void ResetVariables();
        void CreateObject();
        void ClearObjects();
    private:
        bool showObjectEditor;
        bool isRigidObject;
        bool isMovable;
        ID ID;
        float radius;
        pe2d::Vector2 size;
        pe2d::Vector2 position;
        pe2d::Vector2 scale;
        float rotation;
        float mass;
        Color color;
        pe2d::Vector2 velocity;
        pe2d::Vector2 gravity;
    };
}