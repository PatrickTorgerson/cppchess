#pragma once

#include <SFML/Graphics.hpp>

#include "state.hpp"

#include <string>
#include <map>
#include <memory>

class App
{
    sf::RenderWindow win {sf::VideoMode(640, 480), "Chess"};

    std::map<std::string,std::unique_ptr<State>> states;
    State* current_state {nullptr};

public:

    void run(const std::string& state);

    template <typename T, typename ... Args>
    void add_state(const std::string& name, Args&& ... args)
    {
        static_assert(std::is_base_of_v<State, T>, "type T must be derived from State to be added as a state");

        auto& [it,success] = states.insert(
            {
                name,
                std::make_unique<T>(*this, std::forward<Args>(args)...)
            }
        );

        if(!success)
        { throw std::runtime_error("Cannot add duplicate State"); }
    }

    template <typename T>
    T& get_state(const std::string& name)
    {
        static_assert(std::is_base_of_v<State, T>, "type T must be derived from State to be added as a state");

        auto it = states.find(name);

        if(it != states.end())
        {
            return *dynamic_cast<T*>(it->second.get());
        }
        throw std::runtime_error("State does not exist");
    }

    void change_state(const std::string& name);

private:

    void handle_events();
    void update(sf::Time dt);
    void render();
};