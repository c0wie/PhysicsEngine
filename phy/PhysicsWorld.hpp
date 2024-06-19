#pragma once


#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Object.hpp"
#include "Collision.hpp"
#include "PhysicsWorld.hpp"

namespace phy
{
    class PhysicsWorld
    {
    public:
        std::vector< std::shared_ptr<Object> > m_Objects;
        static inline constexpr float m_Gravity = 0.981f;
    public:
        // you can delete this lines but I'm not sure if you want to copy your PhysicsWorld
        PhysicsWorld() = default;
        ~PhysicsWorld() = default;
        PhysicsWorld(const PhysicsWorld &other) = delete;
        PhysicsWorld(PhysicsWorld &&other) = delete;
        PhysicsWorld operator = (const PhysicsWorld &other) = delete;
        PhysicsWorld operator = (PhysicsWorld &&other) = delete;


        void AddObject(std::shared_ptr<Object> object)
        {
            m_Objects.push_back(object);
            std::cout << "Object had been added\n";
        }

        void RemoveObject(std::shared_ptr<Object> object)
        {
            m_Objects.erase( std::remove(m_Objects.begin(), m_Objects.end(), object) );
            std::cout << "Object had been removed\n";
        }

        void Step(float deltaTime)
        {
            for(int i = 0; i < m_Objects.size(); i++)
            {
                if(m_Objects[i]->Transform->Position.y > 500.f)
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

        void ResolveCollision(float delatTime)
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
                    
                    CollisionPoints points = m_Objects[i]->Collider->TestCollision(m_Objects[i]->Transform, m_Objects[j]->Collider, m_Objects[j]->Transform);
                
                    //reconsider if HasCollision == false is not cause to skip whole iteration cuz it means that this object just won't do anything
                    if(points.HasCollision)
                    {
                        collisions.emplace_back(m_Objects[i], m_Objects[j], points);
                    }
                }
            }
        }
        void Draw(sf::RenderWindow &window)
        {
            for(const auto obj : m_Objects)
            {
                sf::RectangleShape body( sf::Vector2f{100.f, 40.f});
                body.setPosition(obj->Transform->Position.x, obj->Transform->Position.y);
                body.setFillColor(sf::Color::Red);
                body.setOrigin( body.getSize().x / 2.f , body.getSize().y / 2.f );
                window.draw(body);
            }
        }
    }; 
}