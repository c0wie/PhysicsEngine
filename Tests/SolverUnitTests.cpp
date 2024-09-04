#include <gtest/gtest.h>
#include "Solver.hpp"

namespace
{
    pe2d::RigidObject CreateObject(std::shared_ptr<pe2d::Collider> collider, Vector2 position, bool isStatic)
    {
        return pe2d::RigidObject(0, collider, pe2d::Transform(position, pe2d::Vector2(1.0f, 1.0f), 0.0), 10.0f, pe2d::Vector2(),
                                isStatic, 0.0f, 0.0f, 0.0f);
    }
    class SolverTest : public testing::Test
    {
    protected:
        void AddCollision(float depth, pe2d::Vector2 normal, pe2d::RigidObject &object1, pe2d::RigidObject &object2)
        {
            pe2d::CollisionPoints points = pe2d::CollisionPoints(normal, depth, Vector2(0.0f, 0.0f), true);
            m_Collisions.emplace_back( pe2d::Collision(object1, object2, points) );
        }

        void PositionSolverTest(const std::vector<pe2d::Vector2> &expectedPositions)
        {
            ASSERT_EQ(m_Collisions.size() * 2, expectedPositions.size());
            PositionSolver(m_Collisions, 0.0f);
            for(int i = 0; i < m_Collisions.size(); i++)
            {
                const Vector2 position1 = m_Collisions[i].GetObjectA().GetPosition();
                const Vector2 position2 = m_Collisions[i].GetObjectB().GetPosition();
                EXPECT_EQ(expectedPositions[i].x, position1.x);
                EXPECT_EQ(expectedPositions[i].y, position1.y);
                EXPECT_EQ(expectedPositions[i + 1].x, position2.x);
                EXPECT_EQ(expectedPositions[i + 1].y, position2.y);
            }
        }

    protected:
        std::vector<pe2d::Collision> m_Collisions;
    };

    TEST_F(SolverTest, positionSolverNonStatic)
    {
        RigidObject object1 = CreateObject(std::make_shared<pe2d::CircleCollider>(40.0f), pe2d::Vector2(200.0f, 200.0f), false);
        RigidObject object2 = CreateObject(std::make_shared<pe2d::BoxCollider>(40.0f, 40.0f), pe2d::Vector2(100.0f, 100.0f), false);
        this->AddCollision(2.0f, pe2d::Vector2(1.0f, 0.0f), object1, object2);
        const std::vector<pe2d::Vector2> expectedPositions =
        {   
            pe2d::Vector2(201.0f, 200.0f),
            pe2d::Vector2(99.0f, 100.0f)
        };  
        this->PositionSolverTest(expectedPositions);
    }

    TEST_F(SolverTest, positionSolverStatic)
    {
        RigidObject object1 = CreateObject(std::make_shared<pe2d::CircleCollider>(40.0f), pe2d::Vector2(200.0f, 200.0f), true);
        RigidObject object2 = CreateObject(std::make_shared<pe2d::BoxCollider>(40.0f, 40.0f), pe2d::Vector2(100.0f, 100.0f), false);
        this->AddCollision(2.0f, pe2d::Vector2(1.0f, 0.0f), object1, object2);
        const std::vector<pe2d::Vector2> expectedPositions =
        {   
            pe2d::Vector2(200.0f, 200.0f),
            pe2d::Vector2(98.0f, 100.0f)
        };  
        this->PositionSolverTest(expectedPositions);
    }
}