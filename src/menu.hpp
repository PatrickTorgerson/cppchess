#pragma once

#include "state.hpp"

#include <string>

class App;

class Menu final : public State
{
public:
    Menu(App& app) : State(app) {}

    virtual void handle_events(const sf::Event& event) override;
    virtual void update(sf::Time dt) override;
    virtual void render(const sf::RenderTarget& target) override;

private:

    void button(const std::string& name, void(Menu::*fn)());

    void host();
    void join();
};