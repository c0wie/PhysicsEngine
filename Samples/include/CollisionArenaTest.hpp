#include "Test.hpp"

namespace test
{
    struct Color
    {
        float red, green, blue;
    };
    enum class ObjectType
    {
        BOX = 0 , 
        CIRCLE = 1,
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
        bool showColorPicker; 
        bool showObjectEditor;
        bool isRigidObject;
        ObjectType objectType;
        float radius;
        pe2d::Vector2 size;
        pe2d::Vector2 position;
        pe2d::Vector2 scale;
        float rotation;
        float mass;
        Color color;
        pe2d::Vector2 velocity;
        pe2d::Vector2 gravity;
        float staticFriction;
        float dynamicFriction;
    public:
        bool wasColliding = false;
    };
}