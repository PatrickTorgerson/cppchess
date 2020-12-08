#include "app.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/System.hpp>


void App::run()
{
    ImGui::SFML::Init(win);

    sf::Clock clock;

    while(win.isOpen())
    {
        handle_events();
        update(clock.restart());
        render();
    }

    ImGui::SFML::Shutdown();
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
        current_state->handle_events(event);
    }
}


void App::update(sf::Time dt)
{
    ImGui::SFML::Update(win, dt);
    current_state->update(dt);
}


void App::render()
{
    win.clear();
    current_state->render(win);
    ImGui::SFML::Render(win);
    win.display();
}