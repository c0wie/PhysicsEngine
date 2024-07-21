#include "BroadPhaseGrid.hpp"

namespace pe2d
{
    BroadPhaseGrid::BroadPhaseGrid() :
        m_ChildNodes
        ({
            nullptr,
            nullptr,
            nullptr,
            nullptr
        }),
        m_TopLeftCorner(Vector2{0.0f, 0.0f}),
        m_BotRightCorner(Vector2{0.0f, 0.0f}),
        m_Depth(0)
    {
        Resize(m_TopLeftCorner, m_BotRightCorner);
    }

    BroadPhaseGrid::BroadPhaseGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int depth) :
        m_ChildNodes
        ({
            nullptr,
            nullptr,
            nullptr,
            nullptr
        }),
        m_TopLeftCorner(topLeftCorner),
        m_BotRightCorner(bottomRightCorner),
        m_Depth(depth)
    {
        Resize(m_TopLeftCorner, m_BotRightCorner);
    }

    bool BroadPhaseGrid::InBoundary(Vector2 boundingBox, Vector2 position, Vector2 leftCorner, Vector2 rightCorner)
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
            if(vertices[i].x < leftCorner.x || vertices[i].x > rightCorner.x 
                || vertices[i].y < leftCorner.y || vertices[i].y > rightCorner.y)
            {
                return false;
            }
        }
        return true;

    }

    void BroadPhaseGrid::Insert(std::shared_ptr<CollisionObject> object)
    {
        if(!object)
        {
            ASSERT("OBJECT CANNOT BE NULL POINTER");
        }   

        for(unsigned char i = 0; i < m_ChildNodes.size(); i++)
        {
            if(InBoundary(object->GetBounadingBox(), object->GetPosition(), m_ChildNodesSize[i].first, m_ChildNodesSize[i].second))
            {
                if(m_Depth + 1 < MAX_DEPTH)
                {
                    if(!m_ChildNodes[i])
                    {
                        m_ChildNodes[i] = std::make_unique<BroadPhaseGrid>
                        (
                            Vector2{m_ChildNodesSize[i].first},
                            Vector2{m_ChildNodesSize[i].second},
                            m_Depth + 1
                        );
                    }
                    m_ChildNodes[i]->Insert(object);
                    return;
                }
            }
        }
        m_Objects.push_back(object);
    }

    void BroadPhaseGrid::GetCollisionPairs(std::vector<std::pair<std::shared_ptr<CollisionObject>, std::shared_ptr<CollisionObject>>> &pairs)
    {
        for(unsigned char i = 0; i < m_ChildNodes.size(); i++)
        {
            if(m_ChildNodes[i])
            {
                m_ChildNodes[i]->GetCollisionPairs(pairs);
            }
        }

        for(int i = 0; i < m_Objects.size(); i++)
        {
            for(int j = i + 1; j < m_Objects.size(); j++)
            {
                if(!m_Objects[i]->GetCollider() || !m_Objects[j]->GetCollider()) // both have colliders
                {
                    continue;
                }
                pairs.emplace_back(std::make_pair(m_Objects[i], m_Objects[j]));
            }
        }
    }

    void BroadPhaseGrid::Resize(Vector2 topLeftCorner, Vector2 bottomRightCorner)
    {
        Clear(); 
        m_TopLeftCorner = topLeftCorner;
        m_BotRightCorner = bottomRightCorner;
        m_ChildNodesSize = 
        {
            std::make_pair(Vector2{m_TopLeftCorner.x, m_TopLeftCorner.y},
                Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f}),
            std::make_pair(Vector2{m_TopLeftCorner.x, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f}, 
                Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, m_BotRightCorner.y}),
            std::make_pair(Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, m_TopLeftCorner.y},
                Vector2{m_BotRightCorner.x, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f}),
            std::make_pair(Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f},
                Vector2{m_BotRightCorner.x, m_BotRightCorner.y})
        };
    }
 
    void BroadPhaseGrid::Clear()
    {
        m_Objects.clear();
        for(unsigned char i = 0; i < m_ChildNodes.size(); i++)
        {
            if(m_ChildNodes[i])
            {
                m_ChildNodes[i]->Clear();
            }
            m_ChildNodes[i].reset();
        }
        
    }
} 