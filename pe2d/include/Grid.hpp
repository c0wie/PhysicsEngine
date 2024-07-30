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
        }
    public:
        void Update()
        {

        }
        
        /*void Clear()
        {
            for(auto &rows : m_Grid)
            {
                rows.clear();
            }
            m_Grid.clear();
        }
    private:
        Vector2 GetObjectPosInGrid(Vector2 objectPosition) const
        {
            const Vector2 gridSize = Vector2( (m_BotRightCorner.x - m_TopLeftCorner.x), (m_BotRightCorner.y - m_TopLeftCorner.y) );
            float cellRow = (objectPosition.x / gridSize.x) * m_CellCount;
            if(cellRow - floor(cellRow) < 0.5f)
            {
                cellRow = floor(cellRow);
            }
            else
            {
                cellRow = ceil(cellRow);
            }
            float cellColumn = (objectPosition.y / gridSize.y) * m_CellCount;
            if(cellColumn- floor(cellColumn) < 0.5f)
            {
                cellColumn = floor(cellColumn);
            }
            else
            {
                cellColumn = ceil(cellColumn);
            }
            return Vector2(cellRow, cellColumn);
        }*/

    private:
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        float m_Width;
        float m_Height;
        float m_CellSize;
    };
}