#pragma once

#include "sfml-forward.hpp"

class State
{
public:

    virtual ~State() = default;

    virtual void handle_events(const sf::Event& event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void render(const sf::RenderTarget& target) = 0;
};