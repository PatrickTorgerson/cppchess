#include "menu.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

void Menu::handle_events(const sf::Event& event)
{}


void Menu::update(sf::Time dt)
{}


void Menu::render(const sf::RenderTarget& target)
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(300,600));
    ImGui::Begin("Demo window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImVec2 bsize(200,20);
    float xpos {(ImGui::GetWindowWidth()/2.0f) - 100.0f};

    ImGui::SetCursorPosX((ImGui::GetWindowWidth()/2.0f) - 20.0f);
    ImGui::Text("Chess");
    ImGui::Separator();

    ImGui::SetCursorPosX(xpos);
    ImGui::Button("Host", bsize);
    ImGui::SetCursorPosX(xpos);
    ImGui::Button("Join", bsize);
    ImGui::SetCursorPosX(xpos);
    ImGui::Button("AI", bsize);

    ImGui::End();
}