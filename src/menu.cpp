#include "menu.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "app.hpp"

#include "game.hpp"

void Menu::handle_events(const sf::Event& event)
{}


void Menu::update(sf::Time dt)
{}


void Menu::render(const sf::RenderTarget& target)
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(300,600));
    ImGui::Begin("Demo window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::SetCursorPosX((ImGui::GetWindowWidth()/2.0f) - 20.0f);
    ImGui::Text("Chess");
    ImGui::Separator();

    button("Host", &Menu::host);
    button("Join", &Menu::join);

    ImGui::End();
}


void Menu::button(const std::string& name, void(Menu::*fn)())
{
    static const float xpos {(ImGui::GetWindowWidth()/2.0f) - 100.0f};
    static const ImVec2 bsize(200,20);

    ImGui::SetCursorPosX(xpos);

    if(ImGui::Button(name.c_str(), bsize))
    { (this->*fn)(); }
}


void Menu::host()
{
    app.change_state("game");
    app.get_state<Game>("game").set_hosting(true);
    app.get_state<Game>("game").start();
}


void Menu::join()
{
    app.change_state("game");
    app.get_state<Game>("game").set_hosting(false);
    app.get_state<Game>("game").start();
}