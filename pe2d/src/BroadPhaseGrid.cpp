#include "BroadPhaseGrid.hpp"

namespace pe2d
{
    BroadPhaseGrid::BroadPhaseGrid() :
        m_TopLeftTree(nullptr),
        m_TopRightTree(nullptr),
        m_BotLeftTree(nullptr),
        m_BotRightTree(nullptr),
        m_TopLeftCorner(Vector2{0.0f, 0.0f}),
        m_BotRightCorner(Vector2{0.0f, 0.0f}),
        m_Precision(0.0f)
    {}

    BroadPhaseGrid::BroadPhaseGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float precision) :
        m_TopLeftTree(nullptr),
        m_TopRightTree(nullptr),
        m_BotLeftTree(nullptr),
        m_BotRightTree(nullptr),
        m_TopLeftCorner(topLeftCorner),
        m_BotRightCorner(bottomRightCorner),
        m_Precision(0.0f)
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
            if(vertices[i].x < m_TopLeftCorner.x && vertices[i].x > m_BotRightCorner.x 
                && vertices[i].y < m_TopLeftCorner.y && vertices[i].y > m_BotRightCorner.y)
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
        if(abs(m_TopLeftCorner.x - m_BotRightCorner.x) <= m_Precision && abs(m_TopLeftCorner.y - m_BotRightCorner.y) <= m_Precision) 
        {
            m_Objects.push_back(object);
            return;
        }

        const Vector2 objectPos = object->GetPosition();

        if( (m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f >= objectPos.x )
        {
            // topLeftTree
            if( (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f >= objectPos.y)
            {
                if(!m_TopLeftTree)
                {
                    m_TopLeftTree = std::make_unique<BroadPhaseGrid>
                    (
                        Vector2{m_TopLeftCorner.x, m_TopLeftCorner.y},
                        Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f},
                        m_Precision
                    );
                }
                m_TopLeftTree->Insert(object);
            }
            // bottomLeftTree
            else
            {
                if(!m_BotLeftTree)
                {
                    m_BotRightTree = std::make_unique<BroadPhaseGrid>
                    (
                        Vector2{m_TopLeftCorner.x, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f},
                        Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, m_BotRightCorner.y},
                        m_Precision
                    );
                }
                m_BotLeftTree->Insert(object);
            }
        }
        else
        {
            // topRightTree
            if( (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f >= objectPos.y)
            {
                if(!m_TopRightTree)
                {
                    m_TopRightTree = std::make_unique<BroadPhaseGrid>
                    (
                        Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, m_TopLeftCorner.y},
                        Vector2{m_BotRightCorner.x, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f},
                        m_Precision
                    );
                }
                m_TopRightTree->Insert(object);
            }
            // bottomRightTree
            else
            {
                if(!m_BotRightTree)
                {
                    m_BotRightTree = std::make_unique<BroadPhaseGrid>
                    (
                        Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f},
                        Vector2{m_BotRightCorner.x, m_BotRightCorner.y},
                        m_Precision
                    );
                }
                m_BotRightTree->Insert(object);
            }
        }
    }
} 
