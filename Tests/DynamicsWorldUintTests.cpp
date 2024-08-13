#include <gtest/gtest.h>
#include "DynamicsWorld.hpp"

namespace
{
    class DynamicsWorldTest : public testing::Test
    {
    protected:
        void SetUp()
        {
            m_Objects[0] = std::make_shared<pe2d::RigidObject>(0, std::make_shared<pe2d::BoxCollider>(100.0f, 100.0f),
                            pe2d::Transform(pe2d::Vector2(600.0f, 100.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f),
                            false, nullptr, 100.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 98.1f),
                            1.0f, 1.0f, 0.0f);
            m_Objects[1] = std::make_shared<pe2d::RigidObject>(1, std::make_shared<pe2d::CircleCollider>(40.0f),
                            pe2d::Transform(pe2d::Vector2(300.0f, 580.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f),
                            false, nullptr, 200.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 98.1f),
                            0.5f, 0.5f, 0.0f);
            m_Objects[2] = std::make_shared<pe2d::RigidObject>(2, std::make_shared<pe2d::BoxCollider>(),
                            pe2d::Transform(pe2d::Vector2(800.0f, 721.0f), pe2d::Vector2(1.0f, 1.0f), 30.0f),
                            false, nullptr, 50.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 9.81f),
                            0.0f, 0.0f, 0.0f);
        }
    protected:
        std::unordered_map<size_t, std::shared_ptr<pe2d::RigidObject>> m_Objects;
    };
}