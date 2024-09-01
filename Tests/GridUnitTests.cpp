#include <gtest/gtest.h>
#include "Grid.hpp"

namespace
{
    class GridTemporary : public pe2d::Grid
    {
    public:
        GridTemporary() : Grid() {}
        GridTemporary& operator=(const Grid &other)
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
        void CheckIfContained(Vector2 point, bool expectedResult)
        {
            const bool result = m_Grid.Contains(point);
            EXPECT_EQ(result, expectedResult);
        }
        void UpdateTest(const std::unordered_map<size_t, RigidObject> &objects, const std::vector<pe2d::Vector2> &expectedTakenCells)
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
        void GetCollisionPairsTest(std::list<std::pair<size_t, size_t>> &expectedIDPairs)
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
        void HasBeenCheckedTest(std::unordered_multimap<size_t, size_t> &checkedPairs, std::pair<size_t, size_t> pair, float expectedResult)
        {
            const bool result = m_Grid.HasBeenChecked(checkedPairs, pair);
            EXPECT_EQ(result, expectedResult);
        }
        RigidObject CreateObject(std::shared_ptr<pe2d::Collider> collider, pe2d::Transform transform, size_t ID)
        {
            return RigidObject(ID, collider, transform, 50.0f, pe2d::Vector2(), false, 0.0, 0.0f, 0.0f);
        }
        void InsertID(int x, int y, size_t ID)
        {
            m_Grid.m_Grid[y][x].push_back(ID);
        }
    protected:
        GridTemporary m_Grid;
    };

    TEST_F(GridTest, contained)
    {
        const Vector2 point1 = pe2d::Vector2(50.0f, 50.0f);
        const Vector2 point2 = pe2d::Vector2(0.0f, 100.0f);
        this->CheckIfContained(point1, true);
        this->CheckIfContained(point2, true);
    }

    TEST_F(GridTest, notContained)
    {
        const Vector2 point1 = pe2d::Vector2(-50.0f, 50.0f);
        const Vector2 point2 = pe2d::Vector2(-1.0f, 100.1f);
        this->CheckIfContained(point1, false);
        this->CheckIfContained(point2, false);
    }

    TEST_F(GridTest, update)
    {
        const std::unordered_map<size_t, RigidObject> objects = 
        {
            {0, CreateObject(std::make_shared<pe2d::CircleCollider>(19.0f), pe2d::Transform(pe2d::Vector2(0.0f, 0.0f),
                            pe2d::Vector2(1.0f, 1.0f), 0.0f), 0)},
            {1, CreateObject(std::make_shared<pe2d::BoxCollider>(15.0f, 15.0f), pe2d::Transform(pe2d::Vector2(50.0f, 50.0f),
                            pe2d::Vector2(1.0f, 1.0f), 45.0f), 1)}
        };
        const std::vector<pe2d::Vector2> expectedTakenCells = 
        {
            pe2d::Vector2(1.0f, 1.0f), pe2d::Vector2(2.0f, 1.0f), pe2d::Vector2(3.0f, 1.0f),
            pe2d::Vector2(1.0f, 2.0f), pe2d::Vector2(2.0f, 2.0f), pe2d::Vector2(3.0f, 2.0f),
            pe2d::Vector2(1.0f, 3.0f), pe2d::Vector2(2.0f, 3.0f), pe2d::Vector2(3.0f, 3.0f),
            pe2d::Vector2(0.0f, 0.0f)
        };
        this->UpdateTest(objects, expectedTakenCells);
    }

    TEST_F(GridTest, getCollisionPairs)
    {
        std::list<std::pair<size_t, size_t>> expectedIDPairs = 
        {
            std::make_pair(0, 2),
            std::make_pair(1, 2)
        };
        this->InsertID(0, 0, 0);
        this->InsertID(0, 0, 2);
        this->InsertID(1, 0, 1);
        this->InsertID(1, 0, 2);
        this->GetCollisionPairsTest(expectedIDPairs);
    }

    TEST_F(GridTest, hasBeenChecked)
    {
        std::unordered_multimap<size_t, size_t> checkedPairs = 
        {
            {1, 2},
            {4, 6},
            {0, 2},
            {6, 9}
        };
        this->HasBeenCheckedTest(checkedPairs, std::make_pair(1, 2), true);
    }

    TEST_F(GridTest, hasNotBeenChecked)
    {
        std::unordered_multimap<size_t, size_t> checkedPairs = 
        {
            {1, 2},
            {4, 6},
            {0, 2},
            {6, 9}
        };
        this->HasBeenCheckedTest(checkedPairs, std::make_pair(4, 8), false);
    }
}