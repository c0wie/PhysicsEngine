#pragma once 

#include <array>
#include <list>
#include "CollisionObject.hpp"
#include "Algo.hpp"

namespace pe2d
{
    template<typename OBJECT_TYPE>
    class QuadTree
    {
    public:
        QuadTree() :
            m_TopLeftCorner(Vector2(0.0f, 0.0f)),
            m_BotRightCorner(Vector2(100.0f, 100.0f)),
            m_MaxDepth(4U)
        {}
        
        QuadTree(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int maxDepth)
        {   
            if(maxDepth == 0)
            {
                ASSERT("Max depth can't be equel 0");
            }
            m_MaxDepth = maxDepth;
            Resize(topLeftCorner, bottomRightCorner);
        }     
    public:
        void Insert(const OBJECT_TYPE &object, const std::vector<Vector2> &vertices)
        {
            for(unsigned char i = 0; i < m_Children.size(); i++)
            {
                if(InBoundary(vertices, m_ChildrenSize[i].first, m_ChildrenSize[i].second))
                {
                    if(m_CurrentDepth + 1 < m_MaxDepth)
                    {
                        if(!m_Children[i])
                        {
                            m_Children[i] = std::make_unique<QuadTree>
                            (
                                Vector2{m_ChildrenSize[i].first},
                                Vector2{m_ChildrenSize[i].second},
                                m_CurrentDepth + 1
                            );
                        }
                        m_Children[i]->Insert(object, vertices);
                        return;
                    }
                }
            }
            m_Items.push_back(object);
        }
        typename std::list< std::pair<OBJECT_TYPE, OBJECT_TYPE> > GetCollisionPairs() const
        {
            std::list< std::pair<OBJECT_TYPE, OBJECT_TYPE> > pairs;
            SearchCollisionPairs(pairs);
            return pairs;
        }
        unsigned int GetSize() const
        {
            unsigned int size = m_Items.size();
            for(int i = 0; i < m_Children.size(); i++)
            {
                if(m_Children[i])
                {
                    size += m_Children[i]->GetSize();
                }
            }
            return size;
        }
        void Resize(Vector2 topLeftCorner, Vector2 bottomRightCorner)
        {
            if(bottomRightCorner.x <= topLeftCorner.x || bottomRightCorner.y <= topLeftCorner.y)
            {
                ASSERT("Unvalid boundary for QuadTree");
            }
            Clear();
            m_TopLeftCorner = topLeftCorner;
            m_BotRightCorner = bottomRightCorner;
            m_ChildrenSize = 
            {
                std::make_pair ( Vector2{m_TopLeftCorner.x, m_TopLeftCorner.y},
                                Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f} ),
                std::make_pair ( Vector2{m_TopLeftCorner.x, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f}, 
                                 Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, m_BotRightCorner.y} ),
                std::make_pair ( Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, m_TopLeftCorner.y},
                                 Vector2{m_BotRightCorner.x, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f} ),
                std::make_pair ( Vector2{(m_TopLeftCorner.x + m_BotRightCorner.x) / 2.0f, (m_TopLeftCorner.y + m_BotRightCorner.y) / 2.0f},
                                Vector2{m_BotRightCorner.x, m_BotRightCorner.y} )
            };
        }
        void Clear()
        {
            m_Items.clear();
            for(unsigned char i = 0; i < m_Children.size(); i++)
            {
                if(m_Children[i])
                {
                    m_Children[i]->Clear();
                }
                m_Children[i].reset();
            }
        }
        bool InBoundary(const std::vector<Vector2> &vertices, Vector2 leftCorner, Vector2 rightCorner) const
        {
            for(int i = 0; i < m_Children.size(); i++)
            {
                if( vertices[i].x < leftCorner.x || vertices[i].x > rightCorner.x || vertices[i].y < leftCorner.y || vertices[i].y > rightCorner.y )
                {
                    return false;
                }
            }
            return true;
        }
        void SearchCollisionPairs(std::list< std::pair<OBJECT_TYPE, OBJECT_TYPE> > &pairs) const
        {
            for(auto objectA : m_Items)
            {
                for(auto objectB : m_Items)
                {
                    if(objectA == objectB)
                    {
                        break;
                    }
                    pairs.emplace_back(std::make_pair(objectA, objectB));
                }
            } 
            for(unsigned char i = 0; i < m_Children.size(); i++)
            {
                if(m_Children[i])
                {
                    m_Children[i]->SearchCollisionPairs(pairs);
                }
            }
        }
    private:
        std::array<std::unique_ptr<QuadTree<OBJECT_TYPE>>, 4> m_Children;
        std::array<std::pair<Vector2, Vector2>, 4> m_ChildrenSize;
        std::list<OBJECT_TYPE> m_Items;
        // info about a boundary of grid
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        unsigned int m_MaxDepth;
        unsigned int m_CurrentDepth;
    };  
} 
