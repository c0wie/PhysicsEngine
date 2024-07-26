#pragma once 

#include <array>
#include <list>
#include "CollisionObject.hpp"
#include "Algo.hpp"

namespace pe2d
{
    template <typename OBJECT_TYPE>
    class QuadTree
    {
    private:
        QuadTree() :
            m_TopLeftCorner(Vector2(0.0f, 0.0f)),
            m_BotRightCorner(Vector2(0.0f, 0.0f)),
            m_MaxDepth(0U)
        {}
        
        QuadTree(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int maxDepth) 
        {   
            if(maxDepth == 0)
            {
                ASSERT("Max depth can't be equel 0");
            }
            m_MaxDepth = maxDepth;
            Resize(m_TopLeftCorner, m_BotRightCorner);
        } 
    private:
        void Insert(std::shared_ptr<CollisionObject> object, const std::vector<Vector2> &vertices)
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
        
        std::vector<std::pair<std::shared_ptr<CollisionObject>, std::shared_ptr<CollisionObject>>> GetCollisionPairs() const
        {
            std::vector<std::pair<std::shared_ptr<CollisionObject>, std::shared_ptr<CollisionObject>>> pairs;
            const unsigned int count = GetCount();
            pairs.reserve( (count * (count - 1)) / 2);
            SearchCollisionPairs(pairs);
            return pairs;
        }

        unsigned int GetCount() const
        {
            unsigned int count = m_Items.size();
            for(int i = 0; i < m_Children.size(); i++)
            {
                if(m_Children[i])
                {
                    count += m_Children[i]->GetCount();
                }
            }
            return count;
        }
        
        void Resize(Vector2 topLeftCorner, Vector2 bottomRightCorner)
        {
            if(bottomRightCorner.x <= topLeftCorner.x || bottomRightCorner.y <= topLeftCorner.y)
            {
                ASSERT("Wrong placement of corners in partitioning system");
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
                if( vertices[i].x < leftCorner.x || vertices[i].x > rightCorner.x 
                    || vertices[i].y < leftCorner.y || vertices[i].y > rightCorner.y)
                {
                    return false;
                }
            }
            return true;
        }
        
        void SearchCollisionPairs(std::vector<std::pair<std::shared_ptr<CollisionObject>, std::shared_ptr<CollisionObject>>> &pairs) const
        {
            for(int i = 0; i < m_Items.size(); i++)
            {
                for(int j = i + 1; j < m_Items.size(); j++)
                {
                    pairs.emplace_back(std::make_pair(m_Items[i], m_Items[j]));
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
        std::array<std::unique_ptr<QuadTree>, 4> m_Children;
        std::array<std::pair<Vector2, Vector2>, 4> m_ChildrenSize;
        std::vector<std::shared_ptr<CollisionObject>> m_Items;
        // info about a boundary of grid
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        unsigned int m_MaxDepth;
        unsigned int m_CurrentDepth;
        friend QuadTreeContainer<OBJECT_TYPE>;
    };  
    
    template<typename OBJECT_TYPE>
    class StaticQuadTreeContainer
    {
        using QuadTreeContainer = std::list<OBJECT_TYPE>
    public:
        StaticQuadTreeContainer() : root() {}
        StaticQuadTreeContainer(Vector2 topLeftCorner, Vector2 botRightCorner, unsigned int maxDepth) : root(topLeftCorner, botRightCorner, maxDepth) {}
    public:
        void Resize(Vector2 topLeftCorner, Vector2 botRightCorner)
        {
            root.Resize(topLeftCorner, botRightCorner);
        }

        size_t size() const 
        {
            return m_AllItems.size();
        }

        bool empty()
        {
            return m_AllItems.empty();
        }

        void clear()
        {
            root.clear();
            m_AllItems.clear();
        }

        typename QuadTreeContainer::iterator begin()
        {
            return m_AllItems.begin();
        }

        typename QuadTreeContainer::iterator end()
        {
            return m_AllItems.end();
        }

        typename QuadTreeContainer::const_iterator cbegin()
        {
            return m_AllItems.cbegin();
        }

        typename QuadTreeContainer::const_iterator cend()
        {
            return m_AllItems.cend();
        }

        void insert(const OBJECT_TYPE &object, const std::vector<Vector2> vertices)
        {
            m_AllItems.push_back(object);
            root.insert(std::prev(m_AllItems.end()));
        }

        std::list<typename QuadTreeContainer::iterator> GetCollisionParis() const
        {
            std::list<typename QuadTreeContainer::iterator> listItemPointers;
            root.SearchCollisionPairs(listItemPointers);
            return listItemPointers;
        }

    private:
        std::list<OBJECT_TYPE> m_AllItems;
        QuadTreeContainer<typename QuadTreeContainer::iterator> root;
    };
} 
