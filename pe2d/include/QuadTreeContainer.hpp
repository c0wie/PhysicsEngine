#pragma once

#include "QuadTree.hpp"

namespace pe2d
{
    /*class QuadTreeContainer
    {
    public:
        QuadTreeContainer() :
            m_Root(std::make_unique<QuadTreeContainer>()) 
        {}
        QuadTreeContainer(Vector2 topLeftCorner, Vector2 botRightCorner, unsigned int maxDepth) :
            m_Root(std::make_unique<QuadTreeContainer>(topLeftCorner, botRightCorner, maxDepth))
        {}
    public:
        void Resize(Vector2 topLeftCorner, Vector2 botRightCorner) { m_Root->Resize(topLeftCorner, botRightCorner); };
        size_t Size() const { return m_AllItems.size(); }
        bool Empty() const { return m_AllItems.empty(); }
        void Clear()
        {
            m_Root->Clear();
            m_AllItems.clear();
        }
        std::list<std::shared_ptr<CollisionObject>>::iterator Begin() { return m_AllItems.begin(); }
        std::list<std::shared_ptr<CollisionObject>>::iterator End() { return m_AllItems.end(); }
        void Insert(std::shared_ptr<CollisionObject> object, const std::vector<Vector2> &vertices)
        {
            m_AllItems.push_back(object);
            m_Root->Insert(std::prev(m_AllItems.end()), vertices);
        }
    private:
        std::list<std::shared_ptr<CollisionObject>> m_AllItems;
        std::unique_ptr<QuadTreeContainer::iterator> m_Root;
    };*/
}