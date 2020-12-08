#include "app.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/System.hpp>


void App::run(const std::string& state)
{
    ImGui::SFML::Init(win);
    sf::Clock clock;

    change_state(state);

    while(win.isOpen())
    {
        handle_events();
        update(clock.restart());
        render();
    }

    ImGui::SFML::Shutdown();
}

void App::change_state(const std::string& name)
{
    current_state = states[name].get();
}


void App::handle_events()
{
    sf::Event event;

    while(win.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            win.close();
        }

        ImGui::SFML::ProcessEvent(event);

        if(current_state)
        { current_state->handle_events(event); }
    }
}


void App::update(sf::Time dt)
{
    ImGui::SFML::Update(win, dt);

    if(current_state)
    { current_state->update(dt); }
}


void App::render()
{
    win.clear();

    if(current_state)
    { current_state->render(win); }


    ImGui::SFML::Render(win);
    win.display();
}