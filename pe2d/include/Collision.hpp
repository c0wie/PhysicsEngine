#pragma once

#include "CollisionObject.hpp"
#include "RigidObject.hpp"

namespace pe2d
{
    class Collision
    {
    public:
        constexpr Collision() : 
            m_ObjectA(nullptr), m_ObjectB(nullptr), m_Points() {}
        Collision(std::shared_ptr<CollisionObject> a, std::shared_ptr<CollisionObject> b, CollisionPoints points);
        Collision(const Collision &other);
        Collision(Collision &&other) noexcept;
        Collision& operator=(const Collision &other) ;
        Collision& operator=(Collision &&other) noexcept;
    public:
        std::shared_ptr<CollisionObject> GetObjectA() const;
        std::shared_ptr<CollisionObject> GetObjectB() const;
        const CollisionPoints& GetPoints() const;
        void SetCollisionObjectA(std::shared_ptr<CollisionObject> a);
        void SetCollisionObjectB(std::shared_ptr<CollisionObject> b);
        void SetPoints(const CollisionPoints &points);
    private: 
        std::shared_ptr<CollisionObject> m_ObjectA;
        std::shared_ptr<CollisionObject> m_ObjectB;
        CollisionPoints m_Points;
    };
}