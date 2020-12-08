#pragma once

#include "sfml-forward.hpp"

class App;

class State
{
protected:
    App& app;
public:
    State(App& a): app{a} {}

    virtual ~State() = default;

    virtual void handle_events(const sf::Event& event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void render(const sf::RenderTarget& target) = 0;
};