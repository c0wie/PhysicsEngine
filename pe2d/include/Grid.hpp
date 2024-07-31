#pragma once

#include <vector>
#include <list>
#include "Vector2.hpp"
#include <CollisionObject.hpp>

namespace pe2d
{
    class Grid
    {
    public:
        Grid() :
            m_TopLeftCorner(Vector2(0.0f, 0.0f)),
            m_BotRightCorner(Vector2(100.0f, 100.0f)),
            m_CellSize(100.0f)
        {   
            m_Height = std::floor((m_BotRightCorner.y - m_TopLeftCorner.y) / m_CellSize);
            m_Width = std::floor((m_BotRightCorner.x - m_TopLeftCorner.x) / m_CellSize);
        }
        Grid(Vector2 topLeftCorner, Vector2 botRightCorner, float cellSize) :
            m_TopLeftCorner(topLeftCorner),
            m_BotRightCorner(botRightCorner),
            m_CellSize(cellSize)
        {
            if(m_CellSize <= 0)
            {
                ASSERT("Cell of one size has to be grater than 0");
            }
            m_Height = std::floor((m_BotRightCorner.y - m_TopLeftCorner.y) / m_CellSize);
            m_Width = std::floor((m_BotRightCorner.x - m_TopLeftCorner.x) / m_CellSize);
            m_Grid.reserve(m_Height);
            for(auto row : m_Grid)
            {
                row.reserve(m_Width);
            }
        }
    public:
        void Insert(size_t ID, const std::vector<Vector2> &boundingBox);

    private:
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        float m_Width;
        float m_Height;
        float m_CellSize;
        std::vector<std::vector<std::list<size_t>>> m_Grid;
    };
}