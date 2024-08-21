#include <gtest/gtest.h>
#include "PhysicsWorld.hpp"

/*namespace
{    
    class DynamicsWorldForTest : public pe2d::DynamicsWorld {
    public:
        using DynamicsWorld::ApplyGravity; 
        using DynamicsWorld::ApplyFriction;
    };
    class DynamicsWorldTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_World.AddRigidObject(std::make_shared<pe2d::RigidObject>(0U, std::make_shared<pe2d::BoxCollider>(100.0f, 100.0f),
                            pe2d::Transform(pe2d::Vector2(600.0f, 300.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f),
                            false, nullptr, 100.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 98.1f),
                            1.0f, 1.0f, 0.0f));
            m_World.AddRigidObject(std::make_shared<pe2d::RigidObject>(1U, std::make_shared<pe2d::CircleCollider>(40.0f),
                            pe2d::Transform(pe2d::Vector2(600.0f, 211.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f),
                            false, nullptr, 200.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 98.1f),
                            0.5f, 0.5f, 0.0f));
            m_World.AddRigidObject(std::make_shared<pe2d::RigidObject>(2U, std::make_shared<pe2d::BoxCollider>(40.0f, 100.0f),
                            pe2d::Transform(pe2d::Vector2(800.0f, 721.0f), pe2d::Vector2(1.0f, 1.0f), 30.0f),
                            false, nullptr, 50.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 98.1f),
                            0.0f, 0.0f, 0.0f));
        }
        void TearDown() override
        {
            m_World.ClearObjects();
        }
        
        void CheckGravity(std::array<pe2d::Vector2, 3> expectedForces)
        {
            m_World.ApplyGravity();
            for(int i = 0; i < 3; i++)
            {
                auto object = std::static_pointer_cast<pe2d::RigidObject>(m_World.At(i));
                EXPECT_EQ(object->GetForce(), expectedForces[i]);
            }
        }

        void CheckFriction(pe2d::Vector2 expectedFriction1, pe2d::Vector2 expectedFriction2)
        {
            std::vector<pe2d::Collision> collisions = 
            {
                pe2d::Collision(m_World.At(1), m_World.At(2), pe2d::CollisionPoints(pe2d::Vector2(0.0f, 1.0f), 0.0f, true)),
            };
            m_World.ApplyFriction(collisions);
            auto object1 = std::static_pointer_cast<pe2d::RigidObject>(m_World.At(1));
            auto object2 = std::static_pointer_cast<pe2d::RigidObject>(m_World.At(2));
            EXPECT_EQ(object1->GetForce(), expectedFriction1);
            EXPECT_EQ(object2->GetForce(), expectedFriction2);
        }
    protected:
        DynamicsWorldForTest m_World;
    };

    TEST_F(DynamicsWorldTest, applyGravity)
    {
        const std::array<pe2d::Vector2, 3> expectedForces =
        {
            pe2d::Vector2(0.0f, 9810.0f),
            pe2d::Vector2(0.0f, 19620.0f),
            pe2d::Vector2(0.0f, 4905.0f)
        };
        this->CheckGravity(expectedForces);
    }

    TEST_F(DynamicsWorldTest, applyFriction)
    {
        const pe2d::Vector2 expectedFriction1 = pe2d::Vector2(0.0f, 0.0f);
        const pe2d::Vector2 expectedFriction2 = pe2d::Vector2(0.0f, 19620.0f);
        this->CheckFriction(expectedFriction1, expectedFriction2);
    }
}*/