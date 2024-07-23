#pragma once 

#include <array>
#include "CollisionObject.hpp"

namespace pe2d
{
    class QuadTree
    {
    public:
        QuadTree();
        QuadTree(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int depth);
    public:
        void Insert(std::shared_ptr<CollisionObject> object);
        std::vector<std::pair<unsigned int, unsigned int>> GetCollisionPairs() const;
        unsigned int GetCount() const;
        void Resize(Vector2 topLeftCorner, Vector2 bottomRightCorner);
        void Clear();
        bool IsValid() const;
    private:
        static bool InBoundary(Vector2 boundingBox, Vector2 position, Vector2 leftCorner, Vector2 rightCorner);
        void GetCollisionPairs(std::vector<std::pair<unsigned int, unsigned int>> &pairs) const;
    private:
        std::array<std::unique_ptr<QuadTree>, 4> m_ChildNodes;
        std::array<std::pair<Vector2, Vector2>, 4> m_ChildNodesSize;
        std::vector<unsigned int> m_ObjectsID;
        // info about a boundary of grid
        Vector2 m_TopLeftCorner;
        Vector2 m_BotRightCorner;
        float m_MaxDepth;
        float m_CurrentDepth;
    };
} 
