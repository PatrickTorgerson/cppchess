#include "menu.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "app.hpp"

#include "game.hpp"

#include <iostream>

void Menu::on_handle_events(const sf::Event& event)
{}


void Menu::on_update(sf::Time dt)
{}


void Menu::on_render(const sf::RenderTarget& target)
{
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(300,600));

    ImGui::Begin("Demo window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::SetCursorPosX((ImGui::GetWindowWidth()/2.0f) - 20.0f);
    ImGui::Text("Chess");
    ImGui::Separator();

    button("Host", &host_flag);
    button("Join", &join_flag);

    ImGui::End();

    if(join_flag)
    { join(); }
    else if(host_flag)
    { host(); }

}


void Menu::button(const std::string& name, void(Menu::*fn)())
{
    static const float xpos {(ImGui::GetWindowWidth()/2.0f) - 100.0f};
    static const ImVec2 bsize(200,20);

    ImGui::SetCursorPosX(xpos);

    if(ImGui::Button(name.c_str(), bsize))
    { (this->*fn)(); }
}


void Menu::button(const std::string& name, bool* flag)
{
    static const float xpos {(ImGui::GetWindowWidth()/2.0f) - 100.0f};
    static const ImVec2 bsize(200,20);

    ImGui::SetCursorPosX(xpos);

    if(ImGui::Button(name.c_str(), bsize))
    { *flag = true; }
}


void Menu::host()
{
    // connection.host(&app.get_state<Game>("game"));
    // app.change_state("game");
    // app.get_state<Game>("game").start();
    ImGui::OpenPopup("Host");
    ImGui::BeginPopupModal("Host");

    switch(connection.get_status())
    {

    case Connection::Status::none:
        ImGui::Text("Choose Color");
        if(ImGui::Button("White"))
        {
            app.get_state<Game>("game").start(true);
            connection.host(&app.get_state<Game>("game"));
        }
        ImGui::SameLine();
        if(ImGui::Button("Black"))
        {
            app.get_state<Game>("game").start(false);
            connection.host(&app.get_state<Game>("game"));
        }
        if(ImGui::Button("Cancel"))
        {
            host_flag = false;
        }
        break;

    case Connection::Status::connecting:
        ImGui::Text("Connecting...");
        break;

    case Connection::Status::connected:
        app.change_state("game");
        break;

    case Connection::Status::failed:
        ImGui::Text("Connection Failed!");
        if(ImGui::Button("OK"))
        {
            connection.reset();
            host_flag = false;
        }
    }

    ImGui::EndPopup();
}


void Menu::join()
{
    ImGui::OpenPopup("Join");
    ImGui::BeginPopupModal("Join");

    switch(connection.get_status())
    {

    case Connection::Status::none:
        ImGui::InputText("Host Ip", ip_buff, 50);
        if(ImGui::Button("Connect"))
        {
            connection.join(&app.get_state<Game>("game"), std::string(ip_buff));
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel"))
        {
            ip_buff[0] = '\0';
            join_flag = false;
        }
        break;

    case Connection::Status::connecting:
        ImGui::Text("Connecting...");
        break;

    case Connection::Status::connected:
        // host's connection sends client it's color
        // clients connection calls game::start
        app.change_state("game");
        break;

    case Connection::Status::failed:
        ImGui::Text("Connection Failed!");
        if(ImGui::Button("OK"))
        {
            connection.reset();
            join_flag = false;
        }
    }

    ImGui::EndPopup();
}