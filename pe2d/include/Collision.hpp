#pragma once

#include "CollisionObject.hpp"
#include "RigidObject.hpp"

namespace pe2d
{
    // class which holds objects and their collision
    class Collision
    {
    public:
        Collision(RigidObject &a, RigidObject &b, const CollisionPoints &points) :
            m_ObjectA(a), m_ObjectB(b), m_Points(points) {}
        Collision(const Collision &other) = default;
        Collision(Collision &&other) noexcept = default;
        Collision& operator=(const Collision &other) = default;
        Collision& operator=(Collision &&other) noexcept = default;
    public:
        RigidObject& GetObjectA() { return m_ObjectA; }
        RigidObject& GetObjectB() { return m_ObjectB; }
        const CollisionPoints& GetPoints() const { return m_Points; }
        void SetCollisionObjectA(const RigidObject &a) { m_ObjectA = a; }
        void SetCollisionObjectB(const RigidObject &b) { m_ObjectB = b; }
        void SetPoints(const CollisionPoints &points) {m_Points = points; }
    private: 
        RigidObject &m_ObjectA;
        RigidObject &m_ObjectB;
        CollisionPoints m_Points;
    };
}