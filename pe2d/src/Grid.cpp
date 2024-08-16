#include "Grid.hpp"

namespace pe2d
{
    Grid::Grid() :
        m_TopLeftCorner(Vector2(0.0f, 0.0f)),
        m_BotRightCorner(Vector2(100.0f, 100.0f)),
        m_CellSize(100.0f)
    {   
        m_Height = std::floor((m_BotRightCorner.y - m_TopLeftCorner.y) / m_CellSize);
        m_Width = std::floor((m_BotRightCorner.x - m_TopLeftCorner.x) / m_CellSize);
        m_Grid.reserve(m_Height);
        for(auto row : m_Grid)
        {
            row.reserve(m_Width);
        }
    }
    
    Grid::Grid(Vector2 topLeftCorner, Vector2 botRightCorner, float cellSize) :
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
    
    Grid::Grid(Grid &&other) :
        m_TopLeftCorner(other.m_TopLeftCorner),
        m_BotRightCorner(other.m_BotRightCorner),
        m_CellSize(other.m_CellSize),
        m_Height(other.m_Height),
        m_Width(other.m_Width),
        m_Grid(other.m_Grid)
    {
        other.m_TopLeftCorner = Vector2(0.0f, 0.0f);
        other.m_BotRightCorner = Vector2(0.0f, 0.0f);
        other.m_CellSize = 0.0f;
        other.m_Height = 0.0f;
        other.m_Width = 0.0f;
        other.m_Grid.clear();
    }
    
    Grid& Grid::operator=(Grid &&other)
    {
        if(this == &other)
        {
            return *this;
        }
        m_TopLeftCorner = other.m_TopLeftCorner;
        m_BotRightCorner = other.m_BotRightCorner;
        m_CellSize = other.m_CellSize;
        m_Height = other.m_Height;
        m_Width = other.m_Width;

        other.m_TopLeftCorner = Vector2(0.0f, 0.0f);
        other.m_BotRightCorner = Vector2(0.0f, 0.0f);
        other.m_CellSize = 0.0f;
        other.m_Height = 0.0f;
        other.m_Width = 0.0f;
        other.m_Grid.clear();

        return *this;
    }

    void Grid::Update(const std::unordered_map<size_t, RigidObject> &objects)
    {
        m_Grid.clear();
        m_Grid.resize(m_Height);
        for(auto it = objects.begin(); it != objects.end(); it++)
        {
            const std::array<Vector2, 4> boundingBox = it->second.GetBounadingBox();
            
            bool isInsideGrid = false;
            for(int i = 0; i < 4; i++)
            {
                if(Contains(boundingBox[i]))
                {
                    isInsideGrid = true;
                    break;
                }
            }
            if(!isInsideGrid)
            {
                continue;
            }
            const Vector2 &objectMin = boundingBox[0];
            const Vector2 &objectMax = boundingBox[2];

            // calculating position of vertices in grid 
            // decrese width and height because containers are 0 index based
            int bodyMinX = static_cast<int>(std::floor((objectMin.x - m_TopLeftCorner.x) / m_CellSize));
            bodyMinX = std::clamp(bodyMinX, 0, (int)m_Width - 1);

            int bodyMaxX = static_cast<int>(std::floor((objectMax.x - m_TopLeftCorner.x) / m_CellSize));
            bodyMaxX = std::clamp(bodyMaxX, 0, (int)m_Width - 1);

            int bodyMinY = static_cast<int>(std::floor((objectMin.y - m_TopLeftCorner.y) / m_CellSize));
            bodyMinY = std::clamp(bodyMinY, 0, (int)m_Height - 1);

            int bodyMaxY = static_cast<int>(std::floor((objectMax.y - m_TopLeftCorner.y)) / m_CellSize);
            bodyMaxY = std::clamp(bodyMaxY, 0, (int)m_Height - 1);
       
            for(int y = bodyMinY; y <= bodyMaxY; y++)
            {
                auto &row = m_Grid[y];
                if(row.empty())
                {
                    row.resize(m_Width);
                } 
                for(int x = bodyMinX; x <= bodyMaxX; x++)
                {
                    row[x].push_back(it->first);
                }
            }
        }
    }

    std::list<std::pair<size_t, size_t>> Grid::GetCollisionPairs() const
    {
        std::unordered_multimap<size_t, size_t> checkedPairs;
        std::list<std::pair<size_t, size_t>> pairs;

        for(auto rowIterator = m_Grid.begin(); rowIterator != m_Grid.end(); ++rowIterator)
        {
            for(auto cellIterator = rowIterator->begin(); cellIterator != rowIterator->end(); ++cellIterator)
            {
                for(auto objectIteratorA = cellIterator->begin(); objectIteratorA != cellIterator->end(); ++objectIteratorA)
                {
                    for(auto objectIteratorB = cellIterator->begin(); objectIteratorB != cellIterator->end(); ++objectIteratorB)
                    {
                        if(objectIteratorA == objectIteratorB)
                        {
                            break;
                        }
                        std::pair<size_t, size_t> pair = *objectIteratorA < *objectIteratorB ? 
                            std::make_pair(*objectIteratorA, *objectIteratorB) :
                            std::make_pair(*objectIteratorB, *objectIteratorA);
                        if(!HasBeenChecked(checkedPairs, pair))
                        {
                            pairs.push_back(pair);
                            checkedPairs.insert(pair);
                        }
                    }
                }
            }
        }
        return pairs;
    }

    bool Grid::HasBeenChecked(std::unordered_multimap<size_t, size_t> &checkedPairs, std::pair<size_t, size_t> pair) const
    {
        auto [ first, second ] = checkedPairs.equal_range(pair.first);
        for(auto &i = first; i != second; ++i)
        {
            if(i->second == pair.second)
            {
                return true;
            }
        }
        return false;
    }
}