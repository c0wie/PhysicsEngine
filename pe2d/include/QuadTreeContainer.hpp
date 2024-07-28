#pragma once

#include "QuadTree.hpp"

namespace pe2d
{
    template<typename OBJECT_TYPE>
    class QuadTreeContainer
    {
    public:
        QuadTreeContainer() {}
        QuadTreeContainer(Vector2 topLeftCorner, Vector2 botRightCorner, unsigned int maxDepth) :
            m_Root(topLeftCorner, botRightCorner, maxDepth)
        {}
    public:
        void Resize(Vector2 topLeftCorner, Vector2 botRightCorner) { m_Root->Resize(topLeftCorner, botRightCorner); };
        size_t Size() const { return m_AllItems.size(); }
        bool Empty() const { return m_AllItems.empty(); }
        void Clear()
        {
            m_Root.Clear();
            m_AllItems.clear();
        }
        typename std::list<OBJECT_TYPE>::iterator Begin() { return m_AllItems.begin(); }
        typename std::list<OBJECT_TYPE>::iterator End() { return m_AllItems.end(); }
        void Insert(OBJECT_TYPE object, const std::vector<Vector2> &vertices)
        {
            m_AllItems.push_back(object);
            m_Root.Insert(std::prev(m_AllItems.end()), vertices);
        }
        std::list< std::pair< typename std::list<OBJECT_TYPE>::iterator, typename std::list<OBJECT_TYPE>::iterator > > GetCollisionPairs() const
        {
            return m_Root.GetCollisionPairs();
        }
    private:
        std::list<OBJECT_TYPE> m_AllItems;
        QuadTree <typename std::list<OBJECT_TYPE>::iterator> m_Root;
    };
}