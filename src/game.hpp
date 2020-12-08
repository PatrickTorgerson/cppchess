#pragma once

#include "state.hpp"
#include <SFML/Network.hpp>

class App;

class Game final : public State
{
    //Network objects
    sf::Packet packet;
    sf::TcpListener listener;
    sf::TcpSocket socket;
    sf::SocketSelector selector;

    std::string ip_address;
    int port = 24377;
    bool hosting = false;
    void start_server();
    void start_client();

public:
    Game(App& app) : State(app) {}
    
    virtual void handle_events(const sf::Event& event) override;
    virtual void update(sf::Time dt) override;
    virtual void render(const sf::RenderTarget& target) override;

    void start();
    void set_hosting(bool host);
};