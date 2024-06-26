#pragma once


#include "CollisionObject.hpp"
#include "RigidObject.hpp"

namespace phy
{
    struct Collision
    {
    private: 
        std::shared_ptr<CollisionObject> m_ObjectA;
        std::shared_ptr<CollisionObject> m_ObjectB;
        CollisionPoints m_Points;
    public:
        constexpr Collision() :
            m_ObjectA(nullptr), m_ObjectB(nullptr), m_Points() {}
        Collision(std::shared_ptr<CollisionObject> a, std::shared_ptr<CollisionObject> b, CollisionPoints points);
        Collision(std::shared_ptr<RigidObject> a, std::shared_ptr<RigidObject> b, CollisionPoints points);
        Collision(const Collision &other);
        Collision(Collision &&other) noexcept;
        Collision& operator=(const Collision &other) ;
        Collision& operator=(Collision &&other) noexcept;

        const std::shared_ptr<CollisionObject> GetCollisionObjectA() const;
        const std::shared_ptr<CollisionObject> GetCollisionObjectB() const;
        const CollisionPoints& GetPoints() const;
        void SetCollisionObjectA(const std::shared_ptr<CollisionObject> a);
        void SetCollisionObjectB(const std::shared_ptr<CollisionObject> b);
        void SetPoints(const CollisionPoints &points);
    };
}