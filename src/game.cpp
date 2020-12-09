#include "game.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "app.hpp"

#include <iostream>

sf::TcpListener listener;
sf::TcpSocket socket;
sf::SocketSelector selector;
sf::Packet packet;

void wait_for_data()
{
    std::scoped_lock lock(mutex);
    if(selector.wait())
    {
        if(selector.isReady(socket))
        {
            socket.receive(packet);
            packet >> board;
            packet.clear();
            std::cout <<board << std::endl;
        }
    }
}

void send_data()
{
    std::scoped_lock lock(mutex);
    packet << board;
    socket.send(packet);
    packet.clear();
}

void network_handling(Game* game,bool server)
{
    
    if(game->get_white())
    {
        game->set_turn(true);
        while(!game->get_move_made())
        {} 
        game->send_data();
    }
    while(true)
    {
        game->wait_for_data();
        game->set_turn(true);
        while(!game->get_move_made())
        {} 
        game->send_data();
    }
}

void Game::start_server()
{
    std::cout<<"Waiting for players...\n";
    listener.listen(port);
    listener.accept(socket);
    std::cout<<"new client at: " << socket.getRemoteAddress() << std::endl;
    white = true;
}

void Game::start_client()
{
    std::cout << "Enter an IP Address to connect to :";
    std::cin >> ip_address;
    sf::Socket::Status status = socket.connect(ip_address, port);
    if(status == sf::Socket::Done)
    {
        std::cout<<"Connect to server at:" << socket.getRemoteAddress() << std::endl;
    }
}


void Game::on_handle_events(const sf::Event& event)
{}


void Game::on_update(sf::Time dt)
{
    if(turn)
    {
        std::cout << "Enter Text:";
        std::cin >> board;
        std::cout <<"\n";
        turn = false;
    }
}


void Game::on_render(const sf::RenderTarget& target)
{
    //display the board
}

void Game::set_hosting(bool host)
{
    hosting = host;
}

void Game::start()
{
    if(hosting)
        start_server();
    else
        start_client();
    selector.add(socket);
    turn = white;
    network_thread = std::thread(network_handling,this,hosting);
}

void Game::choose_color()
{

}

void Game::set_turn(bool t)
{
    std::scoped_lock lock(mutex);
    turn = t;
}
    
bool Game::get_move_made()
{
    std::scoped_lock lock(mutex);
    return turn == false;
}

bool Game::get_white()
{
    std::scoped_lock lock(mutex);
    return white;
}