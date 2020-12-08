#pragma once

#include "sfml-forward.hpp"

#include <mutex>

class App;

class State
{
protected:
    App& app;
    std::mutex mutex;
public:

    State(App& a): app{a} {}
    virtual ~State() = default;

    void handle_events(const sf::Event& event)
    {
        std::scoped_lock lock(mutex);
        on_handle_events(event);
    }
    void update(sf::Time dt)
    {
        std::scoped_lock lock(mutex);
        update(dt);
    }
    void render(const sf::RenderTarget& target)
    {
        std::scoped_lock lock(mutex);
        render(target);
    }

protected:

    virtual void on_handle_events(const sf::Event& event) = 0;
    virtual void on_update(sf::Time dt) = 0;
    virtual void on_render(const sf::RenderTarget& target) = 0;
};