#include "connection.hpp"

#include "game.hpp"

#include <iostream>


Connection::~Connection()
{
    this->stop();
    t.join();
}


void Connection::host(Game* game)
{
    stop_flag = false;
    t = std::thread(start_server, game, this);
}


void Connection::join(Game* game, const std::string& ip)
{
    stop_flag = false;
    t = std::thread(start_client, game, this, ip);
}


void Connection::cancel()
{
    set_status(Status::none);
    stop();
    t.join();
}


void Connection::reset()
{
    set_status(Status::none);
    stop();
    t.join();
}


void Connection::recieve_board(Game* game)
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
            std::cout << b << std::endl; // TODO: remove
        }
    }
}


void Connection::send_board(Game* game)
{
    packet << game->get_board();
    socket.send(packet);
    packet.clear();
}


void Connection::send_color(bool white)
{
    packet << white;
    socket.send(packet);
    packet.clear();

    std::cout << "sending ";
    if(white) std::cout << "White" << std::endl;
    else      std::cout << "Black" << std::endl;
}


// apears to be broken...
bool Connection::recieve_color()
{
    if(selector.wait())
    {
        if(selector.isReady(socket))
        {
            bool color;

            socket.receive(packet);
            packet >> color;
            packet.clear();

            std::cout << "recieving ";
            if(color) std::cout << "White" << std::endl;
            else      std::cout << "Black" << std::endl;

            return color;
        }
    }
    throw std::runtime_error("Error recieving color");
}


void Connection::wait_for_client(bool white)
{
    bool color = recieve_color();

    if(color != white)
    {
        stop();
        set_status(Status::failed);
        return;
    }
}


void Connection::stop()
{
    std::scoped_lock lock {stop_mutex};
    stop_flag = true;
}


bool Connection::should_stop() const
{
    std::scoped_lock lock {stop_mutex};
    return stop_flag;
}


Connection::Status Connection::get_status() const
{
    std::scoped_lock lock {status_mutex};
    return status;
}


void Connection::set_status(Status s)
{
    std::scoped_lock lock {status_mutex};
    status = s;
}


// free functions with access to Connection's private members


void start_server(Game* g, Connection* c)
{
    std::cout << "Waiting for oponent..." << std::endl;
    c->set_status(Connection::Status::connecting);

    c->listener.listen(c->port);
    c->listener.accept(c->socket);

    std::cout << "Found oponent: " << c->socket.getRemoteAddress() << std::endl;
    c->set_status(Connection::Status::connected);

    c->selector.add(c->socket);

    // Tells the client it's color and wait's for it to be ready
    c->send_color(!(g->is_white()));
    c->wait_for_client(!(g->is_white()));

    network_handling(g,c);
}


void start_client(Game* g, Connection* c, const std::string& ip)
{
    std::cout << "Attempting to connect to " << ip << " ..." << std::endl;
    c->set_status(Connection::Status::connecting);

    sf::Socket::Status status = c->socket.connect(ip, c->port, sf::seconds(15));

    if(status == sf::Socket::Done)
    {
        std::cout << "Connected to server at: " << c->socket.getRemoteAddress() << std::endl;
        c->set_status(Connection::Status::connected);
    }
    else
    {
        std::cout << "Failed to connect!" << std::endl;
        c->stop();
        c->set_status(Connection::Status::failed);
    }

    c->selector.add(c->socket);

    g->start(c->recieve_color());

    // send our color to host to signal we are ready
    c->send_color(g->is_white());

    network_handling(g,c);
}


void network_handling(Game* g, Connection* c)
{
    using namespace std::literals::chrono_literals;

    if(c->get_status() == Connection::Status::failed)
    { return; }

    std::cout << "Let the games begin!" << std::endl;

    // first turn
    if(g->is_white())
    {
        g->go();

        while(!g->done())
        { std::this_thread::sleep_for(200ms); }

        c->send_board(g);
    }

    // turn loop
    while(!c->should_stop())
    {
        c->recieve_board(g);

        g->go();

        while(!g->done())
        { std::this_thread::sleep_for(200ms); }

        c->send_board(g);
    }
}