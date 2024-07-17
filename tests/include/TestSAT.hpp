#include "../../pe2d/include/Algo.hpp"
#include "SFML/Graphics.hpp"

namespace test
{
    class BoxCollision
    {
    public:
        BoxCollision() = default;
        BoxCollision(const pe2d::Vector2 &size, const pe2d::Vector2 &position, float rotation);
    public:
        void SetPosition(const pe2d::Vector2 &position);
        void SetRotation(float angle);
        void Draw(sf::RenderWindow &window) const;    
        void Move(const pe2d::Vector2 &offset);
        void Rotate(float angle);

        pe2d::Vector2 GetPosition() const { return position; }
        float GetRotation() const { return rotation; }
    public:
        bool isColliding;
        std::vector<pe2d::Vector2> vertices;
    private:
        float rotation;
        pe2d::Vector2 position;
        pe2d::Vector2 size;
    private:
        void RotateVertices(float angle);
    };

    class TestSAT 
    {
    public:
        TestSAT();
    public:
        void OnUpdate(float deltaTime, const sf::Vector2i &mousePos);   
        void OnRender(sf::RenderWindow &window) ;
        void OnImGuiRender(sf::RenderWindow &window) {} 
    private:
        static bool CheckCollision(BoxCollision &A, BoxCollision &B);
    private:
        std::array<BoxCollision, 2> m_Boxes;
    };
}