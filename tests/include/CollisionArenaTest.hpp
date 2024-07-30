#include "Test.hpp"

namespace test
{
    struct Color
    {
        int red, green, blue;
    };
    enum class ObjectType
    {
        BOX = 0 , 
        CIRCLE = 1,
    };
    class CollisionArenaTest final : public Test
    {
    public:
        CollisionArenaTest(pe2d::Vector2 topLeftCorner, pe2d::Vector2 bottomRightCorner, unsigned int maxDepth);
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
        pe2d::Vector2 topLeftCorner;
        pe2d::Vector2 botRightCorner;
        int maxDepth;
    };
}