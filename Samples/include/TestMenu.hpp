#include <functional>
#include <vector>
#include "Test.hpp"

namespace test
{
    class TestMenu final : public Test
    {
    public:
        TestMenu(Test *&currentTestPtr, pe2d::Vector2 topLeftCorner, pe2d::Vector2 bottomRightCorner, unsigned int maxDepth) : 
            m_CurrentTest(currentTestPtr)
        {}
        ~TestMenu() {};
    public:
        void OnUpdate(float deltaTime, sf::Vector2i mousePos) override {}
        void OnRender(sf::RenderWindow &window) override {}
        void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO& io) override
        {
            for (auto& [name, test] : m_Tests)
            {
	            if(ImGui::Button(name.c_str()) )
	            {
		            m_CurrentTest = test();
	            }
            }
        }

        template<typename T>
        void RegisterTest(const std::string &testName)
        {
            m_Tests.push_back(std::make_pair(testName, []() { return new T(); } ) );
        }
    private:
        std::vector< std::pair<std::string, std::function<Test*()> >> m_Tests;
        
        Test *&m_CurrentTest;
    };
}