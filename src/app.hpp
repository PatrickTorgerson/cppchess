#pragma once

#include <SFML/Graphics.hpp>

#include "state.hpp"
#include "menu.hpp"

class App
{
    sf::RenderWindow win {sf::VideoMode(640, 480), "Chess"};
    State* current_state {new Menu()}; // <-- Memory Leak!

public:

    void run();

private:

    void handle_events();
    void update(sf::Time dt);
    void render();
};