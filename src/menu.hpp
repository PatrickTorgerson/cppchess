#pragma once

#include "state.hpp"

class Menu final : public State
{
public:
    virtual void handle_events(const sf::Event& event) override;
    virtual void update(sf::Time dt) override;
    virtual void render(const sf::RenderTarget& target) override;
};