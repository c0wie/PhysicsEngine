#include <gtest/gtest.h>
#include "Grid.hpp"

namespace
{
    class GridTestable : public pe2d::Grid
    {
    public:
        GridTestable() : Grid() {}
        GridTestable& operator=(const Grid &other)
        {
            if(this == &other)
            {
                return *this;
            }
            pe2d::Grid::operator=(other);
            return *this;
        }
    public:
        using pe2d::Grid::Contains;
        using pe2d::Grid::HasBeenChecked;
        using pe2d::Grid::m_Grid;
    };
    
    class GridTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_Grid = pe2d::Grid(pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(100.0f, 100.0f), 20.0f);
            m_Grid.m_Grid.resize(5);
            for(int i = 0; i < 5; i++)
            {
                m_Grid.m_Grid[i].resize(5);
            }
        }
        void Test_Contains(Vector2 point, bool expectedResult)
        {
            const bool result = m_Grid.Contains(point);
            EXPECT_EQ(result, expectedResult);
        }
        void Test_Update(const std::unordered_map<size_t, RigidObject> &objects, const std::vector<pe2d::Vector2> &expectedTakenCells)
        {
            m_Grid.Update(objects);
            for(int i = 0 ; i< expectedTakenCells.size(); i++)
            {
                const int x = expectedTakenCells[i].x;
                const int y = expectedTakenCells[i].y;
                const bool isEmpty = m_Grid.m_Grid[x][y].empty();
                EXPECT_EQ(isEmpty, false) << x << " " << y << '\n';
            }
        }
        void Test_GetCollisionPairs(std::list<std::pair<size_t, size_t>> &expectedIDPairs)
        {
            auto IDpairs = m_Grid.GetCollisionPairs();
            ASSERT_EQ(IDpairs.size(), expectedIDPairs.size());
            
            auto it1 = IDpairs.begin();
            auto it2 = expectedIDPairs.begin();
            while(it1 != IDpairs.end())
            {
                EXPECT_EQ(it1->first, it2->first);
                EXPECT_EQ(it1->second, it2->second);
                it1++;
                it2++;
            }

        }
        void Test_HasBeenChecked(std::unordered_multimap<size_t, size_t> &checkedPairs, std::pair<size_t, size_t> pair, float expectedResult)
        {
            const bool result = m_Grid.HasBeenChecked(checkedPairs, pair);
            EXPECT_EQ(result, expectedResult);
        }
        void InsertID(int x, int y, size_t ID)
        {
            m_Grid.m_Grid[y][x].push_back(ID);
        }
    protected:
        GridTestable m_Grid;
    };

    TEST_F(GridTest, ContainsTrue)
    {
        const Vector2 point1 = pe2d::Vector2(50.0f, 50.0f);
        const Vector2 point2 = pe2d::Vector2(0.0f, 100.0f);
        this->Test_Contains(point1, true);
        this->Test_Contains(point2, true);
    }

    TEST_F(GridTest, ContainsFalse)
    {
        const Vector2 point1 = pe2d::Vector2(-50.0f, 50.0f);
        const Vector2 point2 = pe2d::Vector2(-1.0f, 100.1f);
        this->Test_Contains(point1, false);
        this->Test_Contains(point2, false);
    }

    TEST_F(GridTest, Update)
    {
        const std::unordered_map<size_t, RigidObject> objects = 
        {
            { 0, RigidObject( 0, std::make_shared<pe2d::CircleCollider>(19.0f), pe2d::Transform({0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f), 50.0f, 
                            {0.0f, 0.0f}, false, 0.0f, 0.0f, 0.0f) },
            { 1, RigidObject( 1, std::make_shared<pe2d::BoxCollider>(15.0f, 15.0f), pe2d::Transform({50.0f, 50.0f}, {1.0f, 1.0f}, 45.0f), 50.0f,
                            {0.0f, 0.0f}, false, 0.0f, 0.0f, 0.0f) }
        };

        const std::vector<pe2d::Vector2> expectedTakenCells = 
        {
            pe2d::Vector2(1.0f, 1.0f), pe2d::Vector2(2.0f, 1.0f), pe2d::Vector2(3.0f, 1.0f),
            pe2d::Vector2(1.0f, 2.0f), pe2d::Vector2(2.0f, 2.0f), pe2d::Vector2(3.0f, 2.0f),
            pe2d::Vector2(1.0f, 3.0f), pe2d::Vector2(2.0f, 3.0f), pe2d::Vector2(3.0f, 3.0f),
            pe2d::Vector2(0.0f, 0.0f)
        };
        this->Test_Update(objects, expectedTakenCells);
    }

    TEST_F(GridTest, GetCollisionPairs)
    {
        std::list<std::pair<size_t, size_t>> expectedIDPairs = 
        {
            std::make_pair(0, 2),
            std::make_pair(1, 2)
        };
        // row | column | ID
        this->InsertID(0, 0, 0);
        this->InsertID(0, 0, 2);
        this->InsertID(1, 0, 1);
        this->InsertID(1, 0, 2);
        this->Test_GetCollisionPairs(expectedIDPairs);
    }

    TEST_F(GridTest, HasBeenCheckedTrue)
    {
        std::unordered_multimap<size_t, size_t> checkedPairs = 
        {
            {1, 2},
            {4, 6},
            {0, 2},
            {6, 9}
        };
        this->Test_HasBeenChecked(checkedPairs, std::make_pair(1, 2), true);
    }

    TEST_F(GridTest, HasBeenCheckedFalse)
    {
        std::unordered_multimap<size_t, size_t> checkedPairs = 
        {
            {1, 2},
            {4, 6},
            {0, 2},
            {6, 9}
        };
        this->Test_HasBeenChecked(checkedPairs, std::make_pair(4, 8), false);
    }
}