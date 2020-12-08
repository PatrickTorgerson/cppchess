#pragma once

#include "state.hpp"

class App;

class Game final : public State
{
public:
    Game(App& app) : State(app) {}

    virtual void handle_events(const sf::Event& event) override;
    virtual void update(sf::Time dt) override;
    virtual void render(const sf::RenderTarget& target) override;
};