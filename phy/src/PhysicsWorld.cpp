#include "../headers/PhysicsWorld.hpp"

namespace phy
{
    void PhysicsWorld::AddObject(const Object& obj)
    {
        std::shared_ptr<Object> object = std::make_shared<Object>(std::move(obj));
        m_Objects.push_back(object);
    }

    void PhysicsWorld::RemoveObject(std::shared_ptr<Object> object)
    {
        m_Objects.erase( std::remove(m_Objects.begin(), m_Objects.end(), object) );
        LogCall("Object has been romoved");
    }

    void PhysicsWorld::Step(float deltaTime)
    {
        ResolveCollision(deltaTime);
        for(int i = 0; i < m_Objects.size(); i++)
        {
            if(m_Objects[i]->Transform->Position.y > 1000.f)
            {
                RemoveObject(m_Objects[i]);
                continue;
            }
            // example of force
            m_Objects[i]->Force.y += m_Objects[i]->Mass * m_Gravity;

            m_Objects[i]->Velocity += m_Objects[i]->Force / m_Objects[i]->Mass * deltaTime;
            m_Objects[i]->Transform->Position += m_Objects[i]->Velocity * deltaTime;

            m_Objects[i]->Force = Vector2(0.0f, 0.0f);
        }
    }

    void PhysicsWorld::ResolveCollision(float delatTime)
    {
        std::vector<Collision> collisions;
        for(int i = 0; i < m_Objects.size(); i++)
        {
            for(int j = 0; j <m_Objects.size(); j++)
            {
                if(m_Objects[i] == m_Objects[j])
                {
                    break;
                }
                
                if(!m_Objects[i]->Collider || !m_Objects[j]->Collider)
                {
                    continue;
                }
                CollisionPoints points = m_Objects[i]->Collider->TestCollision(m_Objects[i]->Transform.get(), m_Objects[j]->Collider.get(), m_Objects[j]->Transform.get());
            
                if(points.HasCollision)
                {
                    collisions.emplace_back(m_Objects[i], m_Objects[j], points);
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                    {
                       std::cout << points.Depth << '\n';
                    }
                }
            }
        }
    }

    void PhysicsWorld::Draw(sf::RenderWindow &window)
    {
        for(auto obj : m_Objects)
        {
            obj->Collider->Draw(window, obj->Transform.get());
        }
    }
}