#pragma once

#include "state.hpp"

#include <string>

class App;

class Menu final : public State
{
public:

    Menu(App& app) : State(app) {}

private:

    virtual void on_handle_events(const sf::Event& event) override;
    virtual void on_update(sf::Time dt) override;
    virtual void on_render(const sf::RenderTarget& target) override;

    void button(const std::string& name, void(Menu::*fn)());

    void host();
    void join();
};