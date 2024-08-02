#pragma once

#include <vector>
#include <list>
#include "Vector2.hpp"
#include <CollisionObject.hpp>
#include <set>

namespace pe2d
{
    // class which devides space in smaller pieces to increse performence with large group of objects
    class Grid
    {
    public:
        Grid();
        Grid(Vector2 topLeftCorner, Vector2 botRightCorner, float cellSize);
        Grid(const Grid &other) = default;
        Grid(Grid &&other);
        Grid& operator=(const Grid &other) = default;
        Grid& operator=(Grid &&other);
    public:
        // inserts objects id to grid in correct places 
        void Update(const std::unordered_map<size_t, std::shared_ptr<CollisionObject>> &objects);
        //returns list of unique id to collision pairs
        std::list<std::pair<size_t, size_t>> GetCollisionPairs() const;
    private:
        // returns true if any of object boundingBox vertex is inside grid
        constexpr bool Contains(Vector2 vertex)
        {
            if(vertex.x < m_TopLeftCorner.x || vertex.x > m_BotRightCorner.x 
                || vertex.y < m_TopLeftCorner.y || vertex.y > m_BotRightCorner.y)
            {
                return false;
            }
            return true;
        }
        // returns true if pair already has been checked
        bool HasBeenChecked(std::unordered_multimap<size_t, size_t> &checkedPairs, std::pair<size_t, size_t> pair) const;
    private:
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        float m_Width;
        float m_Height;
        float m_CellSize;
        std::vector<std::vector<std::vector<size_t>>> m_Grid;
    };
}