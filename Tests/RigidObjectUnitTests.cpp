#include <gtest/gtest.h>
#include "RigidObject.hpp"

namespace
{
    class RigigObjectTest : public testing::Test
    {
    protected:
        void Test_CalculateRotationalInertia(std::shared_ptr<pe2d::Collider> collider, float mass, bool isStatic, float expectedInertia)
        {
            pe2d::RigidObject object(1, collider, pe2d::Transform({0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f), mass, {0.0f, 98.1f}, isStatic, 0.0f, 0.0f, 0.0f);
            EXPECT_EQ(object.GetRotationalInertia(), expectedInertia);
        }

        void Test_GetAABB(std::shared_ptr<pe2d::Collider> collider, pe2d::Transform transform, const std::array<pe2d::Vector2, 4> &expectedVertices)
        {
            pe2d::RigidObject object {1, collider, transform, 50.0f, pe2d::Vector2(), false, 0.0f, 0.0f, 0.0f};
            std::array vertices = object.GetAABB();
            for(int i = 0; i < vertices.size(); i++)
            {
                EXPECT_EQ(vertices[i].x, expectedVertices[i].x);
                EXPECT_EQ(vertices[i].y, expectedVertices[i].y);
            }
        }
    };

    TEST_F(RigigObjectTest, CalculateRotationalInvertiaCircle)
    {
        std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(100.0f);
        const float mass = 120.0f;
        const float expectedInertia = 600000.0f;
        this->Test_CalculateRotationalInertia(collider, mass, false, expectedInertia);
    }

    TEST_F(RigigObjectTest, CalculateRotationalInvertiaBox)
    {
        std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(100.0f, 100.0f);
        const float mass = 120.0f;
        const float expectedInertia = 200000.0f;
        this->Test_CalculateRotationalInertia(collider, mass, false, expectedInertia);
    }   

    TEST_F(RigigObjectTest, CalculateRotationalInvertiaStaticObject)
    {
        std::shared_ptr<pe2d::CircleCollider> circleCollider = std::make_shared<pe2d::CircleCollider>(100.0f);
        std::shared_ptr<pe2d::BoxCollider> boxCollider = std::make_shared<pe2d::BoxCollider>(100.0f, 100.0f);
        const float expectedInertia = pe2dMath::INF;
        const float mass = 120.0f;
        
        this->Test_CalculateRotationalInertia(circleCollider, mass, true, expectedInertia);
        this->Test_CalculateRotationalInertia(boxCollider, mass, true, expectedInertia);
    }

    TEST_F(RigigObjectTest, GetAABBCircle)
    {
        const std::array<pe2d::Vector2, 4> expectedVertieces = 
        {
            pe2d::Vector2(400.0f, 400.0f),
            pe2d::Vector2(600.0f, 400.0f),
            pe2d::Vector2(600.0f, 600.0f),
            pe2d::Vector2(400.0f, 600.0f)
        };
        std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(100.0f);
        pe2d::Transform transform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        this->Test_GetAABB(collider, transform, expectedVertieces);
    }

    TEST_F(RigigObjectTest, GetAABBNonRotatedBox)
    {
        const std::array<pe2d::Vector2, 4> expectedVertieces = 
        {
            pe2d::Vector2(450.0f, 450.0f),
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f)
        };
        std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(100.0f, 100.0f);
        pe2d::Transform transform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        this->Test_GetAABB(collider, transform, expectedVertieces);
    }
    
    TEST_F(RigigObjectTest, GetAABBRotatedBox)
    {
        const std::array<pe2d::Vector2, 4> expectedVertieces = 
        {
            pe2d::Vector2(429.289307f, 429.289307f),
            pe2d::Vector2(570.710693f, 429.289307f),
            pe2d::Vector2(570.710693f, 570.710693f),
            pe2d::Vector2(429.289307f, 570.710693f)
        };
        std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(100.0f, 100.0f);
        pe2d::Transform transform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), pe2dMath::DeegresToRadians(45.0f));
        this->Test_GetAABB(collider, transform, expectedVertieces);
    }
    
}