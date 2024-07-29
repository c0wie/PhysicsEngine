#pragma once

#include "QuadTree.hpp"

namespace pe2d
{
    template<typename OBJECT_TYPE>
    struct QuadTreeItem
    {
        OBJECT_TYPE item;
        QuadTreeItemLocation<typename std::list<QuadTreeItem<OBJECT_TYPE>>::iterator> locator;
    };
    template<typename OBJECT_TYPE>
    class QuadTreeContainer
    {
        using Container = std::list<QuadTreeItem<OBJECT_TYPE>>;
    public:
        QuadTreeContainer() {}
        QuadTreeContainer(Vector2 topLeftCorner, Vector2 botRightCorner, unsigned int maxDepth) :
            m_Root(topLeftCorner, botRightCorner, maxDepth)
        {}
    public:
        void Insert(OBJECT_TYPE object, const std::vector<Vector2> &vertices)
        {
            QuadTreeItem<OBJECT_TYPE> newItem;
            newItem.item = object;
            m_AllItems.push_back(newItem);
            m_AllItems.back().locator = m_Root.Insert(std::prev(m_AllItems.end()), vertices);
        }
        
        void Remove(typename std::list<QuadTreeItem<OBJECT_TYPE>>::iterator &item)
        {
            item->locator.container->erase(item->locator.iterator);
            m_AllItems.erase(item);
        }
        
        std::list< std::pair< typename Container::iterator, typename Container::iterator > > GetCollisionPairs() const
        {
            return m_Root.GetCollisionPairs();
        }
        
        void Relocate(typename Container::iterator item, const std::vector<Vector2> &vertices)
        {
            item->locator.container->erase(item->locator.iterator);
            item-> locator = m_Root.Insert(item, vertices);
        }

        void Resize(Vector2 topLeftCorner, Vector2 botRightCorner) { m_Root.Resize(topLeftCorner, botRightCorner); };
        
        size_t Size() const { return m_AllItems.size(); }
        
        bool Empty() const { return m_AllItems.empty(); }
        
        void Clear()
        {
            m_Root.Clear();
            m_AllItems.clear();
        }
        
        typename Container::iterator Begin() { return m_AllItems.begin(); }
        
        typename Container::iterator End() { return m_AllItems.end(); }
    private:
        Container m_AllItems;
        QuadTree <typename Container::iterator> m_Root;
    };
}