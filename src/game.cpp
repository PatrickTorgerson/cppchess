#include "game.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "app.hpp"

#include <iostream>

// Network state
static sf::TcpListener listener;
static sf::TcpSocket socket;
static sf::SocketSelector selector;
static sf::Packet packet;
constexpr int port = 24377;

void wait_for_data(Game* game)
{
    if(selector.wait())
    {
        if(selector.isReady(socket))
        {
            socket.receive(packet);
            std::string b;
            packet >> b;
            game->set_board(b);
            packet.clear();
            std::cout << b << std::endl;
        }
    }
}

void send_data(Game* game)
{
    packet << game->get_board();
    socket.send(packet);
    packet.clear();
}

void network_handling(Game* game,bool server)
{
    if(server)
    {
        std::cout << "Waiting for players...\n";
        listener.listen(port);
        listener.accept(socket);
        std::cout << "new client at: " << socket.getRemoteAddress() << std::endl;
    }
    else
    {
        std::string ip;
        std::cout << "Enter an IP Address to connect to : ";
        std::cin >> ip;
        sf::Socket::Status status = socket.connect(ip, port);
        if(status == sf::Socket::Done)
        {
            std::cout << "Connect to server at: " << socket.getRemoteAddress() << std::endl;
        }
    }

    selector.add(socket);

    using namespace std::literals::chrono_literals;

    if(game->get_white()) // <- TODO: OPTIMIZE! pass white as param
    {
        game->set_turn(true);
        while(!game->get_move_made())
        {
            std::this_thread::sleep_for(1s);
        }
        send_data(game);
    }
    while(true)
    {
        wait_for_data(game);
        game->set_turn(true);
        while(!game->get_move_made())
        {
            std::this_thread::sleep_for(1s);
        }
        send_data(game);
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

    white = hosting;
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

void Game::set_board(std::string b)
{
    std::scoped_lock lock(mutex);
    board = std::move(b);
}

std::string Game::get_board() const
{
    std::scoped_lock lock(mutex);
    return board;
}

bool Game::get_white()
{
    std::scoped_lock lock(mutex);
    return white;
}