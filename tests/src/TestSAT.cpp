#include "TestSAT.hpp"

namespace test
{
    BoxCollision::BoxCollision(const pe2d::Vector2 &size, const pe2d::Vector2 &position, float rotation) :
        size(size), position(position), rotation(rotation), isColliding(false)
    {
        const float halfSizeX = size.x / 2.0f;
        const float halfSizeY = size.y / 2.0f;
        vertices = 
        {
            pe2d::Vector2{ position.x - halfSizeX, position.y + halfSizeY },
            pe2d::Vector2{ position.x + halfSizeX, position.y + halfSizeY },
            pe2d::Vector2{ position.x + halfSizeX, position.y - halfSizeY },
            pe2d::Vector2{ position.x - halfSizeX, position.y - halfSizeY }
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
        angle *= PI / 180.0f;
        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);
        for(int i = 0; i < vertices.size(); i++)
        {
            auto & vertex = vertices[i];
            const float relativeX = vertex.x - position.x;
            const float relativeY = vertex.y - position.y;
            const float rotatedX = (relativeX * cosAngle) - (relativeY * sinAngle);
            const float rotatedY = (relativeX * sinAngle) + (relativeY * cosAngle);
            vertex = pe2d::Vector2{ rotatedX + position.x, rotatedY + position.y };
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

    TestSAT::TestSAT() :
        m_Box(pe2d::Vector2{ 100.0f, 100.0f}, pe2d::Vector2{ 100.0f, 100.0f}, 45.0f )
    {
        m_Colliders = 
        {
            BoxCollision(pe2d::Vector2{ 100.0f, 200.0f}, pe2d::Vector2{ 700.0f, 200.0f}, 30.0f ),
            BoxCollision(pe2d::Vector2{ 100.0f, 100.0f}, pe2d::Vector2{ 500.0f, 500.0f}, 0.0f )
        };
    }

    void TestSAT::OnUpdate(float deltaTime, const sf::Vector2i &mousePos)
    {
        const pe2d::Vector2 s = m_Box.GetPosition();
        const pe2d::Vector2 end = pe2d::Vector2{ (float)mousePos.x, (float)mousePos.y};
        const pe2d::Vector2 position = pe2d::Vector2::lerp(s, end, 16.0f * deltaTime);

        m_Box.SetPosition(position);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
           m_Box.Rotate(100.0f * deltaTime);
        }

        for(int i = 0; i < m_Colliders.size(); i++)
        {
            pe2d::Vector2 MTV;
            if(CheckCollision(m_Box, m_Colliders[i], MTV) )
            {
                CheckCollision(m_Box, m_Colliders[i], MTV);
                m_Box.Move(MTV);
                m_Colliders[i].isColliding = true;
            }
            else
            {
                m_Colliders[i].isColliding = false;
            }
        }
    }
    
    void TestSAT::OnRender(sf::RenderWindow &window) 
    {
        m_Box.Draw(window);
        for(const auto &collider : m_Colliders)
        {
            collider.Draw(window);
        }
    }

    bool TestSAT::CheckCollision(BoxCollision &A, BoxCollision &B, pe2d::Vector2 &MTV)
    {
        const std::vector<pe2d::Vector2> verticesA = A.vertices;
        const std::vector<pe2d::Vector2> verticesB = B.vertices;
        const std::vector<pe2d::Vector2> axes = GetAxes(verticesA, verticesB);

        // GetAxes

        /*for(int i = 0; i <  verticesA.size(); i++)
        {
            pe2d::Vector2 p1 = verticesA[i];
            pe2d::Vector2 p2 = verticesA[ (i + 1) % verticesA.size() ];
            pe2d::Vector2 edge = p2 - p1;
        }*/

        float overlap = pe2d::Algo::INF;
        const pe2d::Vector2 *smallestAxis = nullptr;

        for(int i = 0; i < axes.size(); i++)
        {
            const pe2d::Vector2 p1 = pe2d::Algo::Project(verticesA, axes[i]);
            const pe2d::Vector2 p2 = pe2d::Algo::Project(verticesB, axes[i]);  
            
            if(!pe2d::Algo::Overlap(p1, p2))
            {
                MTV = pe2d::Vector2{0.0f, 0.0f};
                return false;
            }
            
            const float o = pe2d::Algo::GetOverlap(p1, p2);

            if(o < overlap)
            {
                overlap = o;
                smallestAxis = &axes[i];
            }
        }

        MTV = *smallestAxis * overlap;
        if(MTV.dot(A.GetPosition() - B.GetPosition()) < 0.0f)
        {
            MTV *= -1.0f;
        }

        if(A.GetRotation() != 0.0f)
        {
            std::cout << "MTV: " << MTV.GetString() << '\n';
            std::cout << "Smallest axis: " << smallestAxis->GetString() << '\n';
            std::cout << "Overlap: " << overlap << "\n\n";
        }
        return true;
    }

    std::vector<pe2d::Vector2> GetAxes(const std::vector<pe2d::Vector2> &verticesA, const std::vector<pe2d::Vector2> &verticesB)
    {
        std::vector<pe2d::Vector2> axes;
        for(int i = 0; i < verticesA.size(); i++)
        {
            const pe2d::Vector2 p1 = verticesA[i];
            const pe2d::Vector2 p2 = verticesA[(i + 1) % verticesA.size()];
            const pe2d::Vector2 edge = p1 - p2;
            const pe2d::Vector2 normal = edge.perp().normalized();
            axes.push_back(normal);
        }

        for(int i = 0; i < verticesB.size(); i++)
        {
            const pe2d::Vector2 p1 = verticesB[i];
            const pe2d::Vector2 p2 = verticesB[(i + 1) % verticesB.size()];
            const pe2d::Vector2 edge = p1 - p2;
            const pe2d::Vector2 normal = edge.perp().normalized();
            axes.push_back(normal);
        }
        return axes;
    }
}