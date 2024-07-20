#include "BroadPhaseGrid.hpp"

namespace pe2d
{
    BroadPhaseGrid::BroadPhaseGrid() :
        m_TopLeft(Vector2{0.0f, 0.0f}),
        m_BottomRight(Vector2{0.0f, 0.0f}),
        m_TopLeftTree(nullptr),
        m_TopRightTree(nullptr),
        m_BotLeftTree(nullptr),
        m_BotRightTree(nullptr)
    {}

    BroadPhaseGrid::BroadPhaseGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner) :
        m_TopLeft(topLeftCorner),
        m_BottomRight(bottomRightCorner),
        m_TopLeftTree(nullptr),
        m_TopRightTree(nullptr),
        m_BotLeftTree(nullptr),
        m_BotRightTree(nullptr)
    {}

    bool BroadPhaseGrid::InBoundary(Vector2 boundingBox, Vector2 position)
    {
        Vector2 vertices[4] =
        {
            Vector2{position.x - boundingBox.x, position.y + boundingBox.y},
            Vector2{position.x + boundingBox.x, position.y + boundingBox.y},
            Vector2{position.x + boundingBox.x, position.y - boundingBox.y},
            Vector2{position.x - boundingBox.x, position.y - boundingBox.y}
        };

        for(int i = 0; i < 4; i++)
        {
            if(vertices[i].x < m_TopLeft.x && vertices[i].x > m_BottomRight.x 
                && vertices[i].y < m_TopLeft.y && vertices[i].y > m_BottomRight.y)
            {
                return false;
            }
        }
        return true;

    }

    void BroadPhaseGrid::Insert(std::shared_ptr<CollisionObject> object)
    {
        if(object == nullptr)
        {
            ASSERT("OBJECT CANNOT BE NULL POINTER");
        }   

        // this node isn't in boudaries of node
        if(!InBoundary(object->GetBounadingBox(), object->GetPosition()))
        {
            return;
        }

        // we are in the smallest quadtree node
        if(abs(m_TopLeft.x - m_BottomRight.x) <= 1 && abs(m_TopLeft.y - m_BottomRight.y) <= 1) 
        {
            m_Objects.push_back(object);
            return;
        }
    }
} 
