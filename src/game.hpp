#pragma once

#include "state.hpp"

#include <thread>

class App;

class Game final : public State
{
    std::thread network_thread;
 
    //Network variables
    std::string ip_address;
    std::string board;
    int port = 24377;
    bool hosting = false;

    //Chess Variables
    bool turn = false;
    bool white = false;

    void start_server();
    void start_client();
    void choose_color();

public:
    Game(App& app) : State(app) {}
    
    virtual void on_handle_events(const sf::Event& event) override;
    virtual void on_update(sf::Time dt) override;
    virtual void on_render(const sf::RenderTarget& target) override;

    void start();
    void set_hosting(bool host);
    void set_turn(bool t);
    bool get_move_made();
    bool get_white();
};