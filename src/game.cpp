#include "game.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "app.hpp"

#include <iostream>

void Game::start_server()
{
    //TODO
    //Display text on screen "Waiting for players..."
    std::cout<<"Waiting for players...";
    listener.listen(port);
    listener.accept(socket);
    std::cout<<"new client at: " << socket.getRemoteAddress();

}

void Game::start_client()
{
    std::cout << "Enter an IP Address to connect to :";
    std::cin >> ip_address;
    sf::Socket::Status status = socket.connect(ip_address, port);
    if (status == sf::Socket::Done)
    {
        std::cout<<"Connect to server at:" << ip_address;
    }

}

void Game::handle_events(const sf::Event& event)
{}


void Game::update(sf::Time dt)
{}


void Game::render(const sf::RenderTarget& target)
{}

void Game::set_hosting(bool host)
{
    hosting = host;
}

void Game::start()
{
    if (hosting)
        start_server();
    else
        start_client();
    
}
