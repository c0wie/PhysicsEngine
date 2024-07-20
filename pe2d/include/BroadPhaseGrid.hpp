#pragma once 

#include "CollisionObject.hpp"

namespace pe2d
{
    class BroadPhaseGrid
    {
    public:
        BroadPhaseGrid();
        BroadPhaseGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float precision);
    public:
        void Insert(std::shared_ptr<CollisionObject> object);
        bool InBoundary(Vector2 boundingBox, Vector2 position);
    private:
        std::unique_ptr<BroadPhaseGrid> m_TopLeftTree;
        std::unique_ptr<BroadPhaseGrid> m_TopRightTree;
        std::unique_ptr<BroadPhaseGrid> m_BotLeftTree;
        std::unique_ptr<BroadPhaseGrid> m_BotRightTree;
        std::vector< std::shared_ptr<CollisionObject> > m_Objects;
        // info about a boundary of grid
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        float m_Precision;
    };
} 
