#pragma once

#include "state.hpp"
#include "connection.hpp"

#include <string>

class App;

class Menu final : public State
{
    Connection connection;

    bool join_flag {false};
    bool host_flag {false};

    char ip_buff[50] {'\0'};
public:

    Menu(App& app) : State(app) {}

private:

    virtual void on_handle_events(const sf::Event& event) override;
    virtual void on_update(sf::Time dt) override;
    virtual void on_render(const sf::RenderTarget& target) override;

    void button(const std::string& name, void(Menu::*fn)());
    void button(const std::string& name, bool* flag);

    void host();
    void join();
};