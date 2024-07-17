#include "TestSAT.hpp"

namespace test
{
    BoxCollision::BoxCollision(const pe2d::Vector2 &size, const pe2d::Vector2 &position, float rotation) :
        size(size), position(position), rotation(rotation), isColliding(false)
    {
        const float scaledHalfSizeX = size.x / 2.0f;
        const float scaledHalfSizeY = size.y / 2.0f;
        vertices = 
        {
            pe2d::Vector2{ position.x - scaledHalfSizeX, position.y + scaledHalfSizeY },
            pe2d::Vector2{ position.x + scaledHalfSizeX, position.y + scaledHalfSizeY },
            pe2d::Vector2{ position.x + scaledHalfSizeX, position.y - scaledHalfSizeY },
            pe2d::Vector2{ position.x - scaledHalfSizeX, position.y - scaledHalfSizeY }
        };
        RotateVertices(rotation);
    }
    
    void BoxCollision::SetPosition(const pe2d::Vector2 &position)
    {
        const pe2d::Vector2 deltaPosition = position - this->position;
        for(int i = 0; i < vertices.size(); i++)
        {
            vertices[i] += deltaPosition;
        }
        this->position = position;
    }

    void BoxCollision::SetRotation(float angle)
    {
        const float deltaAngle = angle - rotation;
        RotateVertices(deltaAngle);
        rotation = angle;
    }

    void BoxCollision::Move(const pe2d::Vector2 &offset)
    {
        SetPosition(position + offset);
    }

    void BoxCollision::Rotate(float angle)
    {   
        SetRotation(rotation + angle);
    }

    void BoxCollision::RotateVertices(float angle)
    {
        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);
        for(int i = 0; i < vertices.size(); i++)
        {
            const float relativeX = vertices[i].x - position.x;
            const float relativeY = vertices[i].y - position.y;
            const float rotatedX = (relativeX * cosAngle) - (relativeY * sinAngle);
            const float rotatedY = (relativeX * sinAngle) + (relativeY * cosAngle);
            vertices[i] = pe2d::Vector2{ rotatedX + position.x, rotatedY + position.y };
        }
    
    } 
    
    void BoxCollision::Draw(sf::RenderWindow &window) const
    {
        sf::Color color = isColliding? sf::Color::Red : sf::Color::White;
        sf::RectangleShape box(sf::Vector2f{size.x, size.y});
        box.setPosition(sf::Vector2f{position.x, position.y});
        box.setOrigin(box.getSize() / 2.0f);
        box.setFillColor(color);
        box.setRotation(rotation);
        window.draw(box);
    }

    TestSAT::TestSAT()
    {
        m_Boxes = 
        {
            BoxCollision(pe2d::Vector2{ 100.0f, 100.0f}, pe2d::Vector2{ 100.0f, 100.0f}, 0.0f ),
            BoxCollision(pe2d::Vector2{ 100.0f, 100.0f}, pe2d::Vector2{ 500.0f, 500.0f}, 0.0f )
        };
    }

    void TestSAT::OnUpdate(float deltaTime, const sf::Vector2i &mousePos)
    {
        const pe2d::Vector2 s = m_Boxes[0].GetPosition();
        const pe2d::Vector2 end = pe2d::Vector2{ (float)mousePos.x, (float)mousePos.y};
        const pe2d::Vector2 position = pe2d::Vector2::lerp(s, end, 16.0f * deltaTime);

        m_Boxes[0].SetPosition(position);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
           m_Boxes[0].Rotate(100.0f * deltaTime);
        }
        for(int i = 0; i < m_Boxes.size(); i++)
        {
            BoxCollision &A = m_Boxes[i];
            BoxCollision &B = m_Boxes[(i+1) % m_Boxes.size()];
            bool result = CheckCollision(A, B);

            A.isColliding = result;
            B.isColliding = result;
            
        }
    }
    void TestSAT::OnRender(sf::RenderWindow &window) 
    {
        for(const auto &box : m_Boxes)
        {
            box.Draw(window);
        }
    }
    bool TestSAT::CheckCollision(BoxCollision &A, BoxCollision &B)
    {
        const std::vector<pe2d::Vector2> axesA = pe2d::Algo::GetAxes(A.vertices);
        const std::vector<pe2d::Vector2> axesB = pe2d::Algo::GetAxes(B.vertices);

        for(int i = 0; i < axesA.size(); i++)
        {
            const pe2d::Vector2 pA1 = pe2d::Algo::Project(A.vertices, axesA[i]);
            const pe2d::Vector2 pA2 = pe2d::Algo::Project(B.vertices, axesA[i]);  
            
            if(!pe2d::Algo::Overlap(pA1, pA2))
            {
                return false;
            }
        }

        for(int i = 0; i < axesB.size(); i++)
        {
            const pe2d::Vector2 pB1 = pe2d::Algo::Project(A.vertices, axesB[i]);
            const pe2d::Vector2 pB2 = pe2d::Algo::Project(B.vertices, axesB[i]);  
            
            if(!pe2d::Algo::Overlap(pB1, pB2))
            {
                return false;
            }
        }
        return true;
    }
}