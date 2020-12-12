#pragma once

#include <SFML/Network.hpp>

#include <thread>
#include <mutex>

class Game;

class Connection
{

public:

    enum class Status
    {
        none,        // connection not attempted
        connecting,  // connection in progress
        connected,   // connection succesfull
        failed       // connection failed
    };

private:

    // stop is used by both the
    // main thread and the network thread, thus mutex
    mutable std::mutex stop_mutex;
    bool stop_flag {false};

    mutable std::mutex status_mutex;
    Status status {Status::none};

    std::thread t;

    // main thread NEVER touches any of theese bois
    sf::TcpListener listener;
    sf::TcpSocket socket;
    sf::SocketSelector selector;
    sf::Packet packet;

    static constexpr int port {24377};

public:

    ~Connection();

    // called in main thread;
    void host(Game* game);
    void join(Game* game, const std::string& ip);
    void cancel();
    void reset();

    void recieve_board(Game* game);
    void send_board(Game* game);

    void send_color(bool white);
    bool recieve_color();
    void wait_for_client(bool white);

    void stop();
    bool should_stop() const;

    Status get_status() const;
    void set_status(Status s);

    // free functions with access to Connection's private members
    friend void start_server(Game* g, Connection* c);
    friend void start_client(Game* g, Connection* c, const std::string& ip);
    friend void network_handling(Game* g, Connection* c);
};