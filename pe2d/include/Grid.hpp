#pragma once

#include <vector>
#include <list>
#include <set>
#include "Vector2.hpp"
#include "RigidObject.hpp"

namespace pe2d
{
    /*
        The Grid class handles broad-phase collision detection by dividing space into cells, 
        each containing potential collision pairs of objects. It operates by updating the grid 
        with object positions and retrieving a list of potential collision pairs for further, 
        more detailed collision detection.
    */
    class Grid
    {
    public:
        Grid();

    /*
        Constructor that sets up a grid covering a specified area, 
        defined by the top-left and bottom-right corners, with each cell having the given size.
    */
        Grid(Vector2 topLeftCorner, Vector2 botRightCorner, float cellSize);
        Grid(const Grid &other) = default;
        Grid(Grid &&other);
        Grid& operator=(const Grid &other) = default;
        Grid& operator=(Grid &&other);
    public:
        /*
            Updates the grid with the positions of the objects.
            It places each object's ID in the appropriate grid cell based on its position.
        */
        void Update(const std::unordered_map<size_t, RigidObject> &objects);
        /*
            Retrieves a list of unique pairs of object IDs that are potential collision pairs.
            This list is generated based on objects located within the same or neighboring cells.
            The ID pairs that are beeing retrived always goes: `std::pair(lesser number, higher number)`.
        */
        std::list<std::pair<size_t, size_t>> GetCollisionPairs() const;
    protected:
        // Checks if a vertex is inside the grid's bounds.
        constexpr bool Contains(Vector2 vertex)
        {
            if(vertex.x < m_TopLeftCorner.x || vertex.x > m_BotRightCorner.x 
                || vertex.y < m_TopLeftCorner.y || vertex.y > m_BotRightCorner.y)
            {
                return false;
            }
            return true;
        }
        // Checks if a pair of objects has already been checked for collisions.
        bool HasBeenChecked(std::unordered_multimap<size_t, size_t> &checkedPairs, std::pair<size_t, size_t> pair) const;
    protected:
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        float m_Width;
        float m_Height;
        float m_CellSize;
        std::vector<std::vector<std::vector<size_t>>> m_Grid;
    };
}